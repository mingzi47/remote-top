include_directories(./include)

file(GLOB srcmonitor *.cpp)

add_library(monitor STATIC srcmonitor)

