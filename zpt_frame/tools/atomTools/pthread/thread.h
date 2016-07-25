#ifndef THREAD_H_
#define THRRAD_H_

/*
 * 线程是计算机独立运行的最小单位
 * 同一进程内的线程共享进程的地址空间，创建代价小
 * 线程切换速度快
 * 节约资源，节约时间，提高多处理器效率，改善程序结构
 * 进程和线程最大的区别在于：资源共享
 * 线程只拥有少量运行中不可少的资源：程序计数器，一组寄存器，栈，线程说明信号掩码，局部线程变量和线程说明数据
 */

#include "../sem/locker.h"
#include "../../toolsBase.h"

namespace zpt_frame {

class Thread:public ToolsBase {
    typedef void* (*pFunction)(void*);

private:
    pthread_t _threadId;

public:
    void operator()(pFunction f);
    Thread(pFunction f);
    pthread_t getThreadId() { return _threadId; }
    static void wait(pthread_t a[], int size);
}; //end of class Thread

Thread::Thread(pFunction f) {
    int err = pthread_create(&_threadId, NULL, f, NULL);
    if (err != 0) {
    loop:this->_initState = Failure;
	coutDebugMsg("tools/atomtools/thread", "error: %s", "pthread_create error");
	return ;
    }
}

void Thread::wait(pthread_t a[], int size) {
    for (int i=0; i<size; i++) {
	if (pthread_join(a[i], NULL) != 0) {
	    coutDebugMsg("tools/atomtools/thread", "error: %s", "pthread_join error");
	    continue;
	}
    }
}

} //end of namespace zpt_ser

#endif
