#!/bin/bash



function build_server() {
  cmake -B build && cmake --build build
}

function run_server() {
  build_server && ./build/server/server
}

function build_client() {
  if [[ $1 == "" ]]; then
    cd client && wails build -o rtop
  else 
    cd client && wails build -platform=$1 -o rtop
  fi
}

function run_client() {
  cd client && wails dev
}

function help() {
  echo "
  build_server
  run_server

  build_client windows、windows/amd64、windows/arm64、linux、linux/amd64、linux/arm64
              out_path : client/build/bin/
  run_client wails dev
  "
}

if [[ $1 == "build_server" ]]; then
  build_server
elif [[ $1 == "run_server" ]]; then
  run_server
elif [[ $1 == "build_client" ]]; then
  build_client $2
elif [[ $1 == "run_client" ]]; then
  run_client
elif [[ $1 == "help" ]]; then
  help
else
  echo "not found command, ./build help";
fi

function build_docker() {
  docker build -t remote-top .
  docker run --rm -ti  -v  /proc:/hostinfo/proc -v  /etc/passwd:/hostinfo/etc/passwd  -p 50051:50051 remote-top
}
