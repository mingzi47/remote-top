#include <server.h>

namespace server {

std::shared_mutex RWLock;
monitor::monitor_info monitor_value;

static auto get_cpu_class(cpu::cpu_info &cpu) -> pb::Cpu {
  pb::Cpu res;
  res.set_name(std::move(cpu.cpu_name));
  res.set_core_num(cpu.core_num);
  res.set_cpu_hz(cpu.cpu_hz);
  for (auto v : cpu.cpu_s)
    res.add_cpu_s(v);
  return res;
}

static auto get_mem_class(mem::mem_info &mem) -> pb::Mem {
  pb::Mem res;
  res.set_total(mem.mem_total);
  res.set_free(mem.mem_free);
  res.set_cached(mem.mem_cached);
  res.set_available(mem.mem_cached);
  res.set_swap_total(mem.mem_swap_total);
  res.set_swap_free(mem.mem_swap_free);
  return res;
}

static auto get_net_class(net::net_info &net) -> pb::Net {
  pb::Net res;
  res.set_name(std::move(net.net_name));
  res.set_upload(net.net_upload);
  res.set_upload_s(net.net_upload_s);
  res.set_download(net.net_download);
  res.set_download_s(net.net_download_s);
  return res;
}

static auto get_proc_class(proc::proc_info &proc) -> pb::Proc {
  pb::Proc res;
  res.set_name(std::move(proc.proc_name));
  res.set_usr(std::move(proc.proc_usr));
  res.set_state(std::move(proc.proc_state));
  res.set_pid(proc.proc_pid);
  res.set_mem(proc.proc_mem);
  res.set_thread_num(proc.proc_thread_num);
  res.set_cpu_s(proc.proc_cpu);
  return res;
}

} // namespace server
