include_directories(${PROJECT_SOURCE_DIR}/server/include)
include_directories(${PROJECT_SOURCE_DIR}/server/protos)

# 查找本地Protobuf模块的库信息，实际上CMake就是在找Protobuf-config.cmake文件
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# 查找本地gRPC模块的库信息
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

file(GLOB proto_srcs_hdrs ./protos/*.cc)

# 生成proto 文件并链接库
add_library(rg_grpc_proto ${proto_srcs_hdrs})
target_link_libraries(rg_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# 生成二进制文件并链接库
file(GLOB server_src ${PROJECT_SOURCE_DIR}/server/*.cpp 
${PROJECT_SOURCE_DIR}/server/src/*.cpp)


add_executable(server ${server_src})
target_link_libraries(server
  rg_grpc_proto
  monitor
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)
