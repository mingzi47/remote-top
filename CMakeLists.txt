cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_BUILD_TYPE "DEBUG")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_TOOLCHAIN_FILE )


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("Setting default build type to Release")
endif()

project(remote-top LANGUAGES CXX)


add_subdirectory(lib)

add_subdirectory(server)

# add_executable(test main.cpp)

# target_link_libraries(test monitor)

