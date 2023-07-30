#pragma once
#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <deque>
#include <exception>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

/*
  files
    /proc/cpuinfo
    /proc/stat

    user,
    nice,
    system,
    idle,
    iowait,
    irq,
    softirq,
    stealstolen,
    guest,
*/

namespace cpu {

using u64 = uint64_t;

extern std::vector<u64> old_totals;

struct cpu_info {
  std::string cpu_name{};
  u64 core_num{0};
  double cpu_hz{0};
  std::vector<double> cpu_s{};
};

auto get_cpu_info() -> cpu_info &;
} // namespace cpu
