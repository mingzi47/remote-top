#pragma once
#include <filesystem>
#include <pwd.h>
#include <string>

/*
  files
    /proc/N/status
    /proc/cmdline
*/

namespace rtop {
struct proc {
  int m_pid;
  int m_mem;
  int m_cpu;
  int m_utime;
  int m_stime;
  int m_cutime;
  int m_cstime;
  std::string m_name;
  std::string m_usr;
  std::string m_cmdline;
  std::string m_state;
};

} // namespace rtop
