#include "mem.h"
#include "net.h"
#include "proc.h"
#include <chrono>
#include <cpu.h>
#include <format>
#include <iostream>
#include <thread>
#include <string>

using namespace std::chrono_literals;

int main() {
  cpu::cpu_info cpu;
  for (int i = 1; i <= 10; ++i) {
    cpu = cpu::get_cpu_info();
    std::this_thread::sleep_for(5ms);
  }

  std::cout << std::format("{:s} {:.1f}Hz {:d} core", cpu.cpu_name, cpu.cpu_hz,
                           cpu.core_num)
            << "\n";

  for (int i = 0; i <= cpu.core_num; ++i) {
    std::string name = "";
    if (i == 0)
      name = "cpu";
    else
      name = std::format("cpu{}", i - 1);
    std::cout << std::format("{:s}, {:.2f}%\n", name, cpu.cpu_s[i]);
  }

  auto mem = mem::get_mem_info();

  std::cout << std::format(
      "mem total : {} KB, free : {} KB, available : {} KB\ncache : {} "
      "KB\nswap total: {} KB, free : {} KB\n",
      mem.mem_total, mem.mem_free, mem.mem_available, mem.mem_cached,
      mem.mem_swap_total, mem.mem_swap_free);

  auto nets = net::get_net_info();

  for (auto net : nets) {
    std::cout << std::format("{}, upload : {} KB, download : {} KB\n",
                             net.net_name, net.net_upload, net.net_download);
  }

  auto procs = proc::get_proc_info();
  for (int i = 1; i <= 2; ++i) {
    std::this_thread::sleep_for(200ms);
    procs = proc::get_proc_info();
  }

  for (auto proc : procs) {
    std::cout << std::format(
        "{}, {}, {}, {}, {}, {}, {:.1f}\n", proc.proc_pid, proc.proc_name, proc.proc_usr,
        proc.proc_state, proc.proc_thread_num, proc.proc_mem, proc.proc_cpu);
  }

  std::cout << sysconf(_SC_CLK_TCK) << "\n";
  return 0;
}
