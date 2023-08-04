#pragma once

#include <Monitor.grpc.pb.h>
#include <Monitor.pb.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/security/server_credentials.h>
#include <monitor.h>
#include <shared_mutex>

namespace server {

extern std::shared_mutex RWLock;
extern monitor::monitor_info monitor_value;

class MonitorImpl final : public pb::Monitor::Service {
public:
  auto GetCpu(grpc::ServerContext *context, const pb::Request *request,
                  pb::Cpu *rep) -> grpc::Status override;

  auto GetMem(grpc::ServerContext *context, const pb::Request *request,
                  pb::Mem *rep) -> grpc::Status override;

  auto GetNets(grpc::ServerContext *context, const pb::Request *request,
                  grpc::ServerWriter<pb::Net> *writer) -> grpc::Status override;

  auto GetProcs(grpc::ServerContext *context, const pb::Request *request,
                  grpc::ServerWriter<pb::Proc> *writer) -> grpc::Status override;
};

class Server {
public:
  auto Run() -> void;
};

} // namespace server
