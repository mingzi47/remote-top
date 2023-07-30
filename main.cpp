#include <cpu.h>
#include <format>
#include <iostream>
#include <thread>
#include <chrono>

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
    if (i == 0) name = "cpu";
    else name = std::format("cpu{}", i - 1);
    std::cout << std::format("{:s}, {:.2f}%\n", name, cpu.cpu_s[i]);
  }

  return 0;
}
