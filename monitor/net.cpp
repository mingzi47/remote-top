#include <net.h>

namespace net {

static std::ifstream fread;

std::vector<net_info> nets;

static auto trim(std::string &str) -> void {
  str.erase(0, str.find_first_not_of(char{32}));
  str.erase(str.find_last_not_of(char{32}));
}

auto get_net_info() -> std::vector<net_info> & {
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
    std::string name = str.substr(0, str.find(":"));
    str.erase(0, str.find(":") + 1);
    trim(str);
    u64 upload = std::stoull(str);
    for (int i = 0; i < 8; ++i) {
      str.erase(0, str.find_first_of(" "));
      trim(str);
    }
    u64 download = std::stoull(str);

    if (name == "")
      continue;

    nets.push_back({
        .net_name = name,
        .net_upload = upload,
        .net_download = download,
    });
  }

  fread.close();

  return nets;
}

} // namespace net
