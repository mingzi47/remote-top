#pragma once

#include <Monitor.grpc.pb.h>
#include <Monitor.pb.h>
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
  auto GetMonitor(grpc::ServerContext *context, const pb::Request *req,
                  pb::Reponse *rep) -> grpc::Status override;
};

class Server {
public:
  auto Run() -> void;
};

} // namespace server
