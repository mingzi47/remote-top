#include <net.h>

namespace net {

static std::ifstream fread;

std::vector<net_info> nets;

std::unordered_map<std::string, std::pair<u64, u64>> old_nets{};

net_info new_net{};

auto get_net_info() -> std::vector<net_info> & {
  std::unordered_map<std::string, std::pair<u64, u64>> tmp_nets{};
  if (fread.is_open())
    fread.close();

  fread.open("/proc/net/dev", std::ios_base::in);

  std::string str{};
  int skip = 0;
  while (fread.good()) {
    std::getline(fread, str);
    if (skip < 2) {
      ++skip;
      continue;
    }
    if (str.find(":") == std::string::npos)
      continue;
    str.replace(str.find(':'), 1, " ");
    std::istringstream ist(std::move(str));
    ist >> new_net.net_name;
    ist >> new_net.net_upload;
    for (int i = 0; i < 8; ++i)
      ist >> new_net.net_download;

    if (new_net.net_name == "")
      continue;

    new_net.net_upload_s =
        new_net.net_upload - old_nets[new_net.net_name].first;
    new_net.net_download_s =
        new_net.net_download - old_nets[new_net.net_name].second;

    tmp_nets[new_net.net_name] =
        std::make_pair(new_net.net_upload, new_net.net_download);

    nets.push_back(std::move(new_net));
  }

  tmp_nets.swap(old_nets);

  fread.close();

  return nets;
}

net_info::net_info(net_info &&rvalue)
    : net_name(std::move(rvalue.net_name)), net_upload(rvalue.net_upload),
      net_upload_s(rvalue.net_upload_s), net_download(rvalue.net_download),
      net_download_s(rvalue.net_download_s){};

auto net_info::operator=(net_info &&rvalue) -> void {
  this->net_name = std::move(rvalue.net_name);
  this->net_upload = rvalue.net_upload;
  this->net_upload_s = rvalue.net_upload_s;
  this->net_download = rvalue.net_download;
  this->net_download_s = rvalue.net_download_s;
}

} // namespace net
