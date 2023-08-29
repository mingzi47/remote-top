#include <monitor/monitor.h>

using namespace std::chrono_literals;

namespace monitor {

monitor_info monitor_value{};

auto get_monitor_info() -> monitor_info & {
    monitor_value.cpus = std::move(cpu::get_cpu_info());
    monitor_value.mem = mem::get_mem_info();
    monitor_value.nets = std::move(net::get_net_info());
    monitor_value.procs = std::move(proc::get_proc_info());
    return monitor_value;
}

monitor_info::monitor_info(const monitor_info &rvalue) {
    this->operator=(rvalue);
}

auto monitor_info::operator=(const monitor_info &rvalue) -> void {
    cpus = rvalue.cpus;
    mem = rvalue.mem;
    nets = rvalue.nets;
    procs = rvalue.procs;
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
