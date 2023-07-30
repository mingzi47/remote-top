include_directories(./include)

add_library(monitor STATIC monitor_run.cpp cpu.cpp mem.cpp net.cpp proc.cpp)

