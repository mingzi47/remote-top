#include <chrono>
#include <iostream>
#include <server.h>
#include <thread>

using namespace std::chrono_literals;

auto main() -> int {
    // 每一秒收集一次信息
    std::thread collect([&]() {
        while (true) {
            monitor::get_monitor_info();
            {
                std::unique_lock<std::shared_mutex> Wlock(server::RWLock);
                server::monitor_value = std::move(monitor::monitor_value);
            }
            std::this_thread::sleep_for(1s);
        }
    });
    collect.detach();

    // 启动服务
    server::Server server;
    server.Run();
}
