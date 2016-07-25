#ifndef CONTROLMODULE_H_
#define CONTROLMODULE_H_

#include "common.h"
#include "connectProcess.h"
#include "listenProcess.h"
#include "ioHandlerProcess.h"
#include "workProcess.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;


namespace zpt_frame {

//描述一个进程类，pid是目标子进程的PID，pipefd是父进程和子进程通信用的工具
template <typename CommType=NoNamePipe, typename BufType, unsigned int size>
class _Process {
public:
    _Process() {}

public:
    pid_t pid;
    CommType<BufType, size> comm; 
}; //

class ControlModule:BaseModule {
private: //关于系统的一些属性
    FrameType _frameType; //系统类型

private://关于系统的一些负载值
    unsigned int _totalConnectNum; //系统已连接用户数
    unsigned int _connectErrorNum; //系统连接失败用户数
    unsigned int _connectSuccessNum; //系统连接成功用户数
private:
    SerCommumodule<Control, message> _commu; 
    void* controlMenue(void* f = NULL);


private: //关于连接进程的一些信息
    ModuleState _conState;

private: //关于监听进程的一些信息
    ModuleState _lisState;

private: //关于io进程的一些信息
    ModuleState _ioState;

private: //关于work进程的一些信息
    ModuleState _workState;
   
public:
    ControlModule(FrameType fType = LongConnection);
    ~ControlModule();
    void init() {}
    void* run(void* f = NULL);
};



ControlModule::ControlModule(FrameType fType):_frameType(fType) {
    /*
     * 初始化连接进程
     * 初始化监听进程
     * 初始化IO进程
     * 初始化worker进程
     * 初始化系统参数
     */
    coutDebugMsg("ser_module/controlPorcess", "%s", "总控模块启动");
    pid_t pid = fork();
    if (pid == 0) {
	while (1) {
	    cout << "子进程正在运行" << endl;
	    sleep(2);
	}
    } else if (pid > 0) {
	ConnectProcess _conProcess;
        _conProcess.run();
    } else {
	coutDebugMsg("ser_modules/controlProcess", "error:%s", "fork error");
	exit(1);
    }
    _conState = Normal;
    _lisState = Normal;
    _ioState = Normal;
    _workState = Normal;

    _connectErrorNum = 0; //系统连接失败用户数
    _connectSuccessNum = 0; //系统连接成功用户数
}

ControlModule::~ControlModule() {}

void ControlModule::run() {
    Thread t1(controlMenue());
    Thread t2(_commu.run());
    pthread_t threadId[2];
    threadId[0] = t1.getThreadId();
    threadId[1] = t2.getThreadId();
    Thread::wait(threadId, 2); //等待上述两个线程结束
    return NULL;
}


} // end of namespace zpt_frame

#endif
