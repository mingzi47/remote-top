mkdir -p ./server/protos

protoc -I ./protos/ --grpc_out=./server/protos --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ./protos/Monitor.proto

protoc -I ./protos/ --cpp_out=./server/protos ./protos/Monitor.proto

docker run --rm -ti  -v  /proc:/hostinfo/proc -v  /etc/passwd:/hostinfo/etc/passwd  -p 50051:50051 remote-top


