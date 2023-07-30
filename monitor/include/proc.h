#pragma once
#include <algorithm>
#include <cctype>
#include <cpu.h>
#include <filesystem>
#include <fstream>
#include <pwd.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

/*
  files
    /proc/N/status
    /proc/cmdline
*/

namespace proc {
using u64 = uint64_t;

struct proc_info {
  u64 proc_pid{0};
  u64 proc_mem{0};
  u64 proc_thread_num{0};
  double proc_cpu{0};
  std::string proc_name{};
  std::string proc_usr{};
  std::string proc_state{};
};

auto get_proc_info() -> std::vector<proc_info> &;

} // namespace proc
