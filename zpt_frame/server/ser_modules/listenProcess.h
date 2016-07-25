#ifndef LISTENPROCESS_H_
#define LISTENPROCESS_H_

#include "common.h"

namespace zpt_frame {

class CleanConnection:public BaseModule {
public:
    CleanConnection();
    ~CleanConnection();
    void init();
    void* run(void* f=NULL);
};

CleanConnection::CleanConnection() {

}

CleanConnection::~CleanConnection() {
}

void CleanConnection::init() {

}

void* CleanConnection::run(void* f) {
    return NULL;
}

class ListenProcess:public BaseModule {
private:
    EPOLL		_listenEpoll; //监听Epoll     
    unsigned long 	_listenSize;  //监听用户数目
    SerCommuModule<Listen, Message> _commu; //与其他进程通信模块
    CleanConnection     _cleanCon;

public
    ListenProcess();
    ~ListenProcess();
    void init();
    void* run(void* f = NULL);
}; ///end_of  class ListenProcess


void* ListenProcess::run(void* f) {
    Thread t1(_commu.run()); //通信模块
    Thread t2(_cleanCon()); //清除不活跃连接
    pthread_t threadId[2];
    threadId[0] = t1.getThreadId();
    threadId[1] = t2.getThreadId();
    Thread::wait(threadId, 2); //等待上述两个线程结束
    return NULL;
}

} ///end_of namespace zpt_frame

#endif
