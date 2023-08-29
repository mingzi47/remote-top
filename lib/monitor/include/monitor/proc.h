#pragma once
#include <algorithm>
#include <cctype>
#include <monitor/cpu.h>
#include <filesystem>
#include <fstream>
#include <pwd.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <monitor/global.h>

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

    proc_info() = default;

    proc_info(const proc_info &rvalue);
    auto operator=(const proc_info &rvalue) -> proc_info &;

    proc_info(proc_info &&rvalue) noexcept;
    auto operator=(proc_info &&rvalue) noexcept -> proc_info &;

    auto swap(proc_info &rvalue) noexcept -> void;
};

auto get_proc_info(u64 core_num = 1) -> std::vector<proc_info> &&;

} // namespace proc
