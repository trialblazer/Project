#ifndef IOHANDLERPROCESS_H_
#define IOHANDLERPROCESS_H_

#include "common.h"

namespace zpt_ser {

class IOHandlerProcess:public BaseModule {
private:
    SerCommumodule<IoHandler, message> _commu; //通信模块


public:
    IOHandler();
    ~IOHandler();
    void init();
    void* run(void* f = NULL);
};///end_of class IOHandlerProcess

IOHandlerProcess::IOHandlerProcess() {

}

IOHandlerProcess::~IOHandlerProcess() {

}

void IOHandlerProcess::init() {

}

void* IOHandlerProcess::run(void* f) {
    Thread t1(_commu.run());
    pthread_t threadId[1];
    threadId[0] = t1.getThreadId();
    Thread::wait(threadId, 1); //等待上述两个线程结束
    return NULL;
}

} ///end_of namespace zpt_ser

#endif /// IOHANDLERPROCESS_H_
