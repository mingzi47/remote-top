FROM ubuntu:22.04
CMD bash


ARG DEBIAN_FRONTEND=noninteractive

# c++
RUN apt-get -y update && \
      apt-get -y install git gcc g++ cmake build-essential autoconf libtool pkg-config && \
      mkdir -p /usr/src/tmp /usr/src/grpc-lib && cd /usr/src/tmp && git config --global http.postBuffer 524288000

RUN git clone --recurse-submodules -b v1.56.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
      
RUN cd grpc && \
      mkdir -p cmake/build && \
      cd cmake/build && \
      cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=/usr/src/grpc-lib \
      ../.. && \
      make -j 4 && \
      make install && \
      export PATH="/usr/src/grpc-lib:/usr/src/grpc-lib/bin:/usr/src/grpc-lib/lib:$PATH"

WORKDIR /usr/src/app
COPY . .

RUN cmake -Bbuild -DCMAKE_PREFIX_PATH=/usr/src/grpc-lib && cmake --build build
EXPOSE 50051

CMD ["./build/server/server", "/hostinfo"]


