#include <chrono>
#include <format>
#include <iostream>
#include <monitor/monitor.h>
#include <string>
#include <thread>

using namespace std::chrono_literals;

int main() {
  monitor::get_monitor_info();
  while (true) {
    std::this_thread::sleep_for(1s);
    auto &v = monitor::get_monitor_info();

    std::cout << "\n =============== CPU ============== \n";
    std::cout << v.cpus.cpu_name << " " << v.cpus.core_num << " "
              << v.cpus.cpu_hz << "Hz \n";
    for (int i = 0; const auto &cpu : v.cpus.cpu_s) {
      std::string name;
      if (i == 0)
        name = "cpu";
      else
        name = std::format("cpu{}", i - 1);
      std::cout << std::format("{} : {:.1f}%\n", name, cpu);
      ++i;
    }

    std::cout << "\n =============== MEM ============== \n";

    std::cout << std::format("Total : {} KB, Free : {} KB, Available : {} KB\n"
                             "Cache : {} KB\n"
                             "Swap Total : {} KB, Free : {} KB\n",
                             v.mem.mem_total, v.mem.mem_free,
                             v.mem.mem_available, v.mem.mem_cached,
                             v.mem.mem_swap_total, v.mem.mem_swap_free);

    std::cout << "\n =============== NET ============== \n";

    for (const auto &net : v.nets) {
      std::cout << std::format(
          "{}, Uploads : {} KiB, Downloads: {} KiB, Upload "
          "Total: {} KiB, Download Total : {} KiB\n",
          net.net_name, net.net_upload_s, net.net_download_s, net.net_upload,
          net.net_download);
    }

    std::cout << "\n =============== PROC ============== \n";

    for (const auto &proc : v.procs) {
      std::cout << std::format("{}, {}, {}, {}, {} , {:.1f}%, {}KB\n",
                               proc.proc_pid, proc.proc_name, proc.proc_usr,
                               proc.proc_state, proc.proc_thread_num,
                               proc.proc_cpu, proc.proc_mem);
    }
  }
}
