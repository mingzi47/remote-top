mkdir -p ./server/protos

protoc -I ./protos/ --grpc_out=./server/protos --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/Monitor.proto

protoc -I ./protos/ --cpp_out=./server/protos ./protos/Monitor.proto
