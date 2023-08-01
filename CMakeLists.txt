cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_BUILD_TYPE "DEBUG")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

project(rtop LANGUAGES CXX )

include_directories(${PROJECT_SOURCE_DIR}/monitor/include)
include_directories(${PROJECT_SOURCE_DIR}/server/include)
include_directories(${PROJECT_SOURCE_DIR}/server/protos)

add_subdirectory(monitor)

add_subdirectory(server)

# add_executable(test main.cpp)

# target_link_libraries(test monitor)

