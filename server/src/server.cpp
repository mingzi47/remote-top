#include <server.h>
#include <grpcpp/completion_queue.h>

namespace server {

std::shared_mutex RWLock;
monitor::monitor_info monitor_value;

static auto get_cpu_class(cpu::cpu_info &cpu) -> pb::Cpu {
    pb::Cpu res;
    res.set_name(std::move(cpu.cpu_name));
    res.set_core_num(cpu.core_num);
    res.set_cpu_hz(cpu.cpu_hz);
    for (auto v : cpu.cpu_s) res.add_cpu_s(v);
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

auto MonitorImpl::GetCpu(
    grpc::ServerContext *context, const pb::Request *request, pb::Cpu *rep)
    -> grpc::Status {
    cpu::cpu_info tmp;
    {
        std::shared_lock RLock(RWLock);
        tmp = monitor_value.cpus;
    }
    *rep = get_cpu_class(tmp);
    minilog::info("from : {} get cpu info", context->peer());
    return grpc::Status::OK;
}

auto MonitorImpl::GetMem(
    grpc::ServerContext *context, const pb::Request *request, pb::Mem *rep)
    -> grpc::Status {
    mem::mem_info tmp;
    {
        std::shared_lock RLock(RWLock);
        tmp = monitor_value.mem;
    }
    *rep = get_mem_class(tmp);
    minilog::info("from : {} get mem info", context->peer());
    return grpc::Status::OK;
}

auto MonitorImpl::GetNets(
    grpc::ServerContext *context,
    const pb::Request *request,
    grpc::ServerWriter<pb::Net> *writer) -> grpc::Status {
    std::vector<net::net_info> tmp;
    {
        std::shared_lock RLock(RWLock);
        tmp = monitor_value.nets;
    }
    for (auto &net : tmp) { writer->Write(get_net_class(net)); }
    minilog::info("from : {} get net info", context->peer());
    return grpc::Status::OK;
}

auto MonitorImpl::GetProcs(
    grpc::ServerContext *context,
    const pb::Request *request,
    grpc::ServerWriter<pb::Proc> *writer) -> grpc::Status {
    std::vector<proc::proc_info> tmp;
    {
        std::shared_lock RLock(RWLock);
        tmp = monitor_value.procs;
    }
    for (auto &proc : tmp) { writer->Write(get_proc_class(proc)); }
    minilog::info("from : {} get proc info", context->peer());
    return grpc::Status::OK;
}
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
