#pragma once
#include <cstdint>
#include <fstream>
#include <monitor/global.h>
#include <string>

/*
  files
    /proc/meminfo
*/

namespace mem {
struct mem_info {
    using u64 = uint64_t;
    u64 mem_total;
    u64 mem_free;
    u64 mem_available;
    u64 mem_cached;
    u64 mem_swap_total;
    u64 mem_swap_free;
};

auto get_mem_info() -> mem_info;
} // namespace mem
