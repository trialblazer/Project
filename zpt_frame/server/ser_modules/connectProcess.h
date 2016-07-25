#ifndef CONNECTPROCESS_H_
#define CONNECTPROCESS_H_

/*
 * 该模块为整个系统的第一部分，主要负责监听外来连接，并把接收的用户文件描述符交给下一模块
 * 子模块划分：1.负责监听外来连接    2.负责与总控进程进行数据交互    3.负责与监听模块（下一模块）进行数据交互
 */

#include "common.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>

using namespace std;

namespace zpt_frame {

#define DEBUGCONNECT(x) { \
    coutDebugMsg("ser_Modules/connectPorcess", "error: %s", x); \
}
#define SOCKLEN  ((socklen_t)sizeof(struct sockaddr))

/*
 * 1.连接模块中的监听部分
 */
class _Connect:public BaseModule {
    int			_listenSocket;   	    //监听Socket
    struct sockaddr_in	_listenAddr;                //监听Socket地址
    int _listenPort;				    //监听端口
    
    unsigned long _connectNum; //已连接用户量
    unsigned long _errConNum;  //连接失败用户量

public:
   _Connect();
   ~_Connect() { close(_listenSocket); }
   void* run(void* f = NULL);

private:
   void init();
    
private: //总控进程通信消息处理相关函数
   void sendConNum();   //总控进程请求获取连接用户量的请求，处理函数
   void sendErrNum();
   void sendState();
   void sendStartTime();
   void addConNum() { _connectNum++; } 
   void delConNum() { _connectNum--; }
   void addErrNum() { _errConNum++; }
};


void _Connect::sendNum() {
//总控进程请求获取连接用户量的请求，处理函数
}

_Connect::_Connect() {
    _connectNum = 0;
    _errConNum = 0;

    init();
    _listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_listenSocket < 0) {
	DEBUGCONNECT("connect error! listen error.");
	_mdState = Normal;
	return ;
    }
    memset(&_listenAddr, '\0', SOCKLEN);
    _listenAddr.sin_family = AF_INET;
    _listenAddr.sin_port = htons(_listenPort);
    _listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ( (bind(_listenSocket, (struct sockaddr*)&_listenAddr, SOCKLEN) < 0)) {
	DEBUGCONNECT("connect error! listen socket bind error.");
	_mdState = Normal;
    }
    if ((listen(_listenSocket, 5)) < 0) {
	DEBUGCONNECT("connect error! listen socket listen error.");
    };
    coutDebugMsg("ser_modules/connect", "%s", "开始监听");
}


void _Connect::init() {
    _listenPort = LISTENPORT;
}

void* _Connect::run(void* f) {
    socklen_t len = sizeof(struct sockaddr);
    while (_mdCurState == Run) {
	coutDebugMsg("server/ser_models", "%s", "连接模块启动");
	sleep(2);
	int _cli = accept(_listenSocket, (struct sockaddr*)&_listenAddr, &len);
	if (_cli == -1) {
	   _errConNum++;
	} else {//将该用户发送给监听模块
	    Message msg(Ordinary, &cli, sizeof(int));
	    _commuListen[nextListenProcess()].sendMsg(msg, msg.getSize());
	}
	coutDebugMsg("server/ser_modules", "connectSuccess: %u/tconnect failure: %u", _connectNum, _errConNum);
    }
    return NULL;
}

// #define MaxListenProcessSize 4
class ConnectProcess:public BaseModule {
private:
private://功能实现类
    _Connect _con;      //监听功能实现类
    SerCommumodule<Connect, message> _commu;

private://保存变量
    ModuleCurState _commuListenState[MaxListenProcessSize]; //监听进程状态
    ListenModulePattern _LMPattern[MaxListenProcessSize]; //监听进程工作模式
    unsigned long _LMListenSize[MaxListenProcessSize]; //各监听进程监听用户数目

private:
    int nextListenProcess(ListenModulePattern pattern);  //下一个要分配连接用户的进程

public:
    ConnectProcess();           //初始化
    ~ConnectProcess();          //
    void* run(void* f=NULL);
    void init() {}
}; /// end_of class connectProcess



ConnectProcess::ConnectProcess() {
    if (Abnormal == _con._mdState || _commu._mdState == Abnormal)
	this->_mdState = Abnormal;

    for (int i=0; i<MaxListenProcessSize; i++) {
	_commuListenSatate[i] = Stop; //监听进程状态, 停止
	_LMPattern[i] = LongCon;//监听进程工作模式，长连接
	_LMListenSize[i] = 0; //各监听进程监听用户数目，无监听
    }
}

ConnectProcess::~ConnectProcess() {
    for (int i=0; i<MaxListenProcessSize; i++) {
	if (_commuListen[i] != NULL) {
	    delete _commuListen[i];
	    _commuListen[i] = NULL;
	}
    }
}


int ConnectProcess::nextListenProcess(ListenModulePattern pattern) { //下一个要分配连接用户的进程
    int index = -1;
    for (int i=0; i<MaxListenProcessSize; i++) {
	if (_commuListenState[i] == Run) {
	    if (_LMPattern[i] == pattern) {
		if (index == -1 || _LMListenSize[i] < _LMListenSize[index])
		    index = i;
	    }
	}
    }
    return index;
}	

void* ConnectProcess::run(void* f = NULL) {
    /*
     * 建立线程1，运行监听模块
     * 建立线程2，运行与总控通信模块, 运行与监听进程通信模块
     */
    Thread t1(_con.run());
    Thread t2(_commu.run());
    pthread_t threadId[2];
    threadId[0] = t1.getThreadId();
    threadId[1] = t2.getThreadId();
    Thread::wait(threadId, 2); //等待上述两个线程结束
    return NULL;
}


} ///end_of namespace zpt_frame

#endif
