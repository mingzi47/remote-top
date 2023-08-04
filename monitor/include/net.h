#pragma once
#include <cstdint>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

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

    net_info() = default;

    net_info(const net_info &rvalue);
    auto operator=(const net_info &rvalue) -> void;

    net_info(net_info &&rvalue) noexcept;
    auto operator=(net_info &&rvalue) noexcept -> void;

    auto swap(net_info &rvalue) noexcept -> void;
};

auto get_net_info() -> std::vector<net_info> &;

} // namespace net
