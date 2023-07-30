#pragma once
#include <cstdint>

/*
  files
    /proc/meminfo
*/

namespace rtop {
struct mem {
  using i64 = int64_t;
  i64 m_total;
  i64 m_free;
  i64 m_available;
  i64 m_cached;
  i64 m_swap_used;
  i64 m_swap_free;
};
} // namespace rtop
