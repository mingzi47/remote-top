#include <cpu.h>
#include <mem.h>
#include <net.h>
#include <proc.h>

namespace monitor {

struct monitor_info;

extern monitor_info monitor_value;

struct monitor_info {
  cpu::cpu_info cpus{};
  mem::mem_info mem{};
  std::vector<net::net_info> nets{};
  std::vector<proc::proc_info> procs{};

  monitor_info() = default;

  monitor_info(const monitor_info &rvalue);
  auto operator=(const monitor_info &rvalue) -> void;

  monitor_info(monitor_info &&rvalue) noexcept;
  auto operator=(monitor_info &&rvalue) noexcept -> void;

  auto swap(monitor_info &rvalue) noexcept -> void;
};

auto get_monitor_info() -> monitor_info&;

} // namespace monitor
