#include <server.h>

namespace server {

std::shared_mutex RWLock;

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

// MonitorImpl

auto MonitorImpl::GetMonitor(grpc::ServerContext *context,
                             const pb::Request *req, pb::Reponse *rep)
    -> grpc::Status {
  monitor::monitor_info res{};
  {
    std::shared_lock<std::shared_mutex> lock(RWLock);
    res = monitor::get_monitor_info();
  }
  pb::Cpu cpu = get_cpu_class(res.cpus);
  pb::Mem mem = get_mem_class(res.mem);

  rep->set_allocated_cpu(&cpu);
  rep->set_allocated_mem(&mem);

  for (auto &net : res.nets) {
    pb::Net *tmp = rep->add_net();
    *tmp = get_net_class(net);
  }

  for (auto &proc : res.procs) {
    pb::Proc *tmp = rep->add_proc();
    *tmp = get_proc_class(proc);
  }

  return grpc::Status::OK;
}

// Server

auto Server::Run() -> void {
  std::string server_address = "0.0.0.0:50051";

  MonitorImpl service;

  grpc::ServerBuilder builder;

  // 不使用安全认证
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::cout << "Server listening on " << server_address << "\n";
  server->Wait();
}

} // namespace server
