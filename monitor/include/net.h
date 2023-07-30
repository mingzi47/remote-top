#pragma once
#include <cstdint>
#include <vector>
#include <fstream>

/*
  files
    /proc/net/dev
*/

namespace net {
using u64 = uint64_t;

struct net_info {
  std::string net_name{};
  u64 net_upload{0};
  u64 net_upload_s{0};
  u64 net_download{0};
  u64 net_download_s{0};
};

auto get_net_info() -> std::vector<net_info>&;

} // namespace net
