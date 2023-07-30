#include <monitor.h>

using namespace std::chrono_literals;

namespace monitor {

monitor_info value{};

auto get_monitor_info() -> monitor_info& {

  value.cpus = std::move(cpu::get_cpu_info());
  value.mem = mem::get_mem_info();
  value.nets = std::move(net::get_net_info());
  value.procs = std::move(proc::get_proc_info());
  return value;
}

monitor_info::monitor_info(monitor_info &&rvalue) noexcept {
  this->swap(rvalue);
}
auto monitor_info::operator=(monitor_info &&rvalue) noexcept -> void {
  this->swap(rvalue);
}

auto monitor_info::swap(monitor_info &rvalue) noexcept -> void {
  this->cpus = std::move(rvalue.cpus);
  this->mem = rvalue.mem;
  this->nets = std::move(rvalue.nets);
  this->procs = std::move(rvalue.procs);
}
} // namespace monitor
