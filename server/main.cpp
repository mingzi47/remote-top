#include <chrono>
#include <iostream>
#include <server.h>
#include <thread>
#include <global.h>
#include <unistd.h>
#include <cmdline.h>

using namespace std::chrono_literals;

auto main(int argc, char *argv[]) -> int {
    cmdline::parse(
        argc, argv,
        cmdline::option(
            "--help", false,
            [&](std::string value = "") {
                std::cout << std::format(
                    "USAGE : \n\t--port 50051 --path '/' [default : port=50051, "
                    "path='/']\n\nOPTIONS : \n\t--port \n\t--path\n");
                exit(0);
            }),
        cmdline::option(
            "--port", true,
            [](std::string value) { global::g_port = std::stod(value); }),
        cmdline::option(
            "--path", true, [](std::string value) { global::g_path = value; }));

    // 每一秒收集一次信息
    std::thread collect([&]() {
        minilog::info("collect thread start ... ");
        while (true) {
            monitor::get_monitor_info();
            {
                std::unique_lock<std::shared_mutex> Wlock(server::RWLock);
                server::monitor_value = std::move(monitor::monitor_value);
            }
            std::this_thread::sleep_for(1s);
        }
        minilog::info("collect thread stop ... ");
    });
    collect.detach();

    // 启动服务
    server::Server server;
    server.Run();
}
