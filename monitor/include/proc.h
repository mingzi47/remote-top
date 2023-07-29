#pragma once
#include <string>
#include <filesystem>
#include <pwd.h>

namespace rtop {
struct proc {
  int m_pid;
  int m_mem;
  int m_cpu;
  int m_net;
  std::string m_name;
  std::string m_usr;
  std::string m_cmdline;
  std::string m_state;

  proc();

  auto info();
};

} // namespace rtop
