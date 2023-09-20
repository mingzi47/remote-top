## issue

- module(monitor::cpu) :
    - Old data is still used after the number of CPUs is changed

- Exception

# remote-top

Remote Linux Monitoring with [gRPC](https://grpc.io/docs/languages/cpp/quickstart/) Implementation, Client built using [go-wails](https://github.com/wailsapp/wails)

![](./client/img/截屏2023-08-04%2022.19.14.png)

## Why

 

## Todo

- [x] Connectiong to Remote Linux.
- [x] Client connecting to multiple remote Linux instances.
- [ ] async log
- [ ] Establishing a Secure Connection.

## Trying

- [gRPC C++ Environment](https://grpc.io/docs/languages/cpp/quickstart/)

- [go](https://go.dev/), [wails](https://github.com/wailsapp/wails), [vue3](https://vuejs.org), [nodejs](https://nodejs.org)

### server

```bash
$ git clone https://github.com/mingzi47/remote-top.git
$ cd remote-top
$ cmake -B build
$ cmake --build build
$ ./build/server/server
```

### client

```client
$ git clone https://github.com/mingzi47/remote-top.git
$ cd remote-top/client
$ wails dev
```
