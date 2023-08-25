package main

import (
	"rtop/pb"
	"context"
	"fmt"
	"io"
  "errors"

	"google.golang.org/grpc"
)

type Cpu struct {
  Name  string
  MainData float64
  CpuHz float64
  CoreData []KVarr
}

type KVarr struct {
  Name  string
  Value float64
  Color string
}

type Mem struct {
  MemTotal int64
  MemFree int64
  SwapTotal int64
  SwapFree int64
}

type Net struct {
  Name string
  UploadS int64
  DownloadS int64
  Upload int64
  Download int64
}

type Proc struct {
  Name string
  User string
  State string
  Pid int64
  Mem int64
  Thread int64
  Cpu float64
}

type MonitorInfo struct {
  Cpu Cpu
  Mem Mem
  Nets []Net
  Procs []Proc
}

type Collect struct {
  conn map[string]*grpc.ClientConn
  client map[string]pb.MonitorClient
  ctx context.Context
  conn_num int
}

func NewCollect() *Collect {
  return &Collect{}
}

func (collect *Collect) startup(ctx context.Context) {
  collect.ctx = ctx 
  collect.conn_num = 10
  collect.client = make(map[string]pb.MonitorClient)
  collect.conn = make(map[string]*grpc.ClientConn)
}

func (collect *Collect) Delete(ip string, port int32) {
  address := fmt.Sprintf("%s:%d", ip, port)
  collect.conn[address].Close()
  delete(collect.conn, address);
  delete(collect.client, address);
}

func (collect *Collect) Close() {
  for c := range collect.conn {
    collect.conn[c].Close()
    delete(collect.conn, c);
    delete(collect.client, c);
  }
}

func (collect *Collect) shutdown() {
  collect.Close();
}

func (collect *Collect) CreateConn(ip string, port int32) (*pb.MonitorClient, error) {
  if len(collect.client) >= collect.conn_num {
    return nil, errors.New("No free Connect")
  }
  address := fmt.Sprintf("%s:%d", ip, port);
  temp, exist := collect.client[address];
  if exist {
    return &temp, nil;
  }
  conn, err := grpc.Dial(address, grpc.WithInsecure())

  if err != nil {
    return nil, err;
  }

  collect.conn[address] = conn
  collect.client[address] = pb.NewMonitorClient(conn);
  return nil, nil
}


func (collect *Collect)GetCpu(ip string, port int32) (*Cpu, error) {

  address := fmt.Sprintf("%s:%d", ip, port)
  client, ok := collect.client[address];
  if !ok {
    return nil, errors.New("not found");
  }
  res, err := client.GetCpu(context.Background(), &pb.Request{})
  if err != nil {
    return nil, err
  }

  var kvArray []KVarr
  for i, val := range res.CpuS[1:] {
    name := fmt.Sprintf("cpu%d", i)
    kvArray = append(kvArray, KVarr{Name: name, Value: float64(val)})
  }
  return &Cpu{
    Name : res.Name,
    MainData : float64(res.CpuS[0]),
    CpuHz : float64(res.CpuHz),
    CoreData : kvArray,
  }, nil
}


func (collect *Collect) GetMem(ip string, port int32) (*Mem, error) {
  address := fmt.Sprintf("%s:%d", ip, port)
  client, ok := collect.client[address];
  if !ok {
    return nil, errors.New("not found");
  }
  res, err := client.GetMem(context.Background(), &pb.Request{})
  if err != nil {
    return nil, err
  }
  return &Mem{
    MemTotal : res.Total,
    MemFree  : res.Free,
    SwapTotal : res.SwapTotal,
    SwapFree : res.SwapFree,
  }, nil
}

func (collect *Collect)GetNets(ip string, port int32) ([]Net, error) {
  address := fmt.Sprintf("%s:%d", ip, port)
  client, ok := collect.client[address];
  if !ok {
    return nil, errors.New("not found");
  }
  rep, err := client.GetNets(context.Background(), &pb.Request{})
  if err != nil {
    return nil, err
  }
  var res []Net
  for {
    data, err := rep.Recv();
    if err != nil && err == io.EOF {
      break;
    }
    if err != nil {
      return nil, err
    }
    res = append(res, Net{
      Name: data.Name,
      UploadS : data.UploadS,
      DownloadS : data.DownloadS,
      Upload : data.Upload,
      Download : data.Download,
    });
  }
  return res, nil
}

func (collect *Collect)GetProcs(ip string, port int32) ([]Proc, error) {
  address := fmt.Sprintf("%s:%d", ip, port)
  client, ok := collect.client[address];
  if !ok {
    return nil, errors.New("not found");
  }
  rep, err := client.GetProcs(context.Background(), &pb.Request{})
  if err != nil {
    return nil, err
  }
  var res []Proc
  for {
    data, err := rep.Recv();
    if err != nil && err == io.EOF {
      break;
    }
    if err != nil {
      return nil, err
    }
    res = append(res, Proc{
      Name: data.Name,
      User : data.Usr,
      State : data.State,
      Pid : data.Pid,
      Mem : data.Mem,
      Cpu : float64(data.CpuS),
      Thread : data.ThreadNum,
    });
  }
  return res, nil
}
