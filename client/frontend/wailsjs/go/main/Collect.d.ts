// Cynhyrchwyd y ffeil hon yn awtomatig. PEIDIWCH Â MODIWL
// This file is automatically generated. DO NOT EDIT
import {pb} from '../models';
import {main} from '../models';

export function Close():Promise<void>;

export function CreateConn(arg1:string,arg2:number):Promise<pb.MonitorClient>;

export function Delete(arg1:string,arg2:number):Promise<void>;

export function GetCpu(arg1:string,arg2:number):Promise<main.Cpu>;

export function GetMem(arg1:string,arg2:number):Promise<main.Mem>;

export function GetNets(arg1:string,arg2:number):Promise<Array<main.Net>>;

export function GetProcs(arg1:string,arg2:number):Promise<Array<main.Proc>>;
