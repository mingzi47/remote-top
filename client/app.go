package main

import (
	"changeme/pb"
	"context"
	"fmt"
	"io"
	"log"

	"google.golang.org/grpc"
)

// App struct
type App struct {
	ctx context.Context
}

// NewApp creates a new App application struct
func NewApp() *App {
	return &App{}
}

// startup is called when the app starts. The context is saved
// so we can call the runtime methods
func (a *App) startup(ctx context.Context) {
	a.ctx = ctx
}

// Greet returns a greeting for the given name
func (a *App) Greet(name string) string {
	return fmt.Sprintf("Hello %s, It's show time!", name)
}

type Collect struct {
  conn  *grpc.ClientConn
  client pb.MonitorClient
}

func (collect *Collect)Close() {
  collect.Close()
}

func (collect Collect)NewCollect(ip string, port int32) (Collect, error) {
  conn, err := grpc.Dial(fmt.Sprintf("%s:%d", ip, port), grpc.WithInsecure())
  if err != nil {
    log.Fatal(err.Error());
    return Collect{}, err
  }

  // defer conn.Close()

  client := pb.NewMonitorClient(conn);

  if err != nil {
    log.Fatal(err.Error());
    return Collect{}, err
  }

  return Collect{
    conn : conn,
    client : client,
  }, nil
}

func getCpu(client *pb.MonitorClient) (Cpu, error) {

  res, err := (*client).GetCpu(context.Background(), &pb.Request{})
  if err != nil {
    return Cpu{}, err
  }
  var kvArray []KVarr
  for i, val := range res.CpuS[1:] {
    name := fmt.Sprintf("cpu%d", i)
    kvArray = append(kvArray, KVarr{Name: name, Value: float64(val)})
  }
  return Cpu{
    Name : res.Name,
    MainData : float64(res.CpuS[0]),
    CpuHz : float64(res.CpuHz),
    CoreData : kvArray,
  }, nil
}

func (collect Collect)Collect(ip string, port int32) (MonitorInfo, error) {
  conn, err := grpc.Dial(fmt.Sprintf("%s:%d", ip, port), grpc.WithInsecure())
  if err != nil {
    return MonitorInfo{}, err;
  }

  client := pb.NewMonitorClient(conn);
	defer conn.Close()

  if err != nil {
    log.Fatal(err.Error());
    return MonitorInfo{}, err
  }
  cpu, err:= getCpu(&client);
  if err != nil {
    return MonitorInfo{}, err
  }
  mem, err:= getMem(&client);
  if err != nil {
    return MonitorInfo{}, err
  }
  nets, err:= getNets(&client);
  if err != nil {
    return MonitorInfo{}, err
  }
  procs, err:= getProcs(&client);
  if err != nil {
    return MonitorInfo{}, err
  }
  return MonitorInfo{
    Cpu: cpu,
    Mem: mem,
    Nets: nets,
    Procs: procs,
  }, nil
}

func getMem(client *pb.MonitorClient) (Mem, error) {
  res, err := (*client).GetMem(context.Background(), &pb.Request{})
  if err != nil {
    return Mem{}, err
  }
  return Mem{
    MemTotal : res.Total,
    MemFree  : res.Free,
    SwapTotal : res.SwapTotal,
    SwapFree : res.SwapFree,
  }, nil
}

func getNets(client *pb.MonitorClient) ([]Net, error) {
  rep, err := (*client).GetNets(context.Background(), &pb.Request{})
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

func getProcs(client *pb.MonitorClient) ([]Proc, error) {
  rep, err := (*client).GetProcs(context.Background(), &pb.Request{})
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
      Usr : data.Usr,
      State : data.State,
      Pid : data.Pid,
      Mem : data.Mem,
      Cpu : float64(data.CpuS),
      Thread : data.ThreadNum,
    });
  }
  return res, nil
}

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
  Usr string
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
