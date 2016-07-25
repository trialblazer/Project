#ifndef PROCESSPOOL_H_
#define PROCESSPOOL_H_


#include "../atomTools/shm/shm.h"
#include "../debug/debug.h"
#include "prCommunicateionModule.h"
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>
#include <.

namespace zpt_frame {

class Process {
public:
    pid_t   pid;

    int	    pipeFd[2];

public:
    Process():pid(-1) {}
};

template <typename T>
class ProcessPool {
private:
    static const int MaxProcessNum = 16;//进程池中允许最大进程数
    static const int UserPerProcess =  65535;//每个子进程最多处理客户数量
    int processNum; //进程池中进程总数
    int idx;//子进程在池中的序号，从0开始
    int stop;//是否停止运行
    Process* subProcess;//保存所有子进程的描述信息
    static ProcessPooll<T>* instance;//进程池的静态实例

private:
    ProcessPool(int processN = 8);

public:
    static processPool<T>* create(int processN = 8);
    ~ProcessPool() {
	delete [] instance;
    }
    void run(); //启动进程池

private:
    void setUpSigPipe();
    void runParent();
    void runChild();
};

template <typename T>
ProcessPool<T>* ProcessPool<T>::instance = NULL;
 

template <typename T>
ProcessPool<T>::ProcessPool(int processN):processNumber(processN), idx(-1), stop(false) {
    assert((processNum > 0) && (processNum <= MaxProcessNum));
    subProcess = new Process[processNum];
    assert(subProcess);
    //创建一定数量的进程，并建立他们和父进程间的管道
    for (int i=0; i<processNum; i++) {
	subProcess[i].pid = fork();
	assert(subProcess[i].pid >= 0);
	if (subProcess[i].pid > 0) {
	    //维护管道
	} else if (subProcess[i].pid == 0) {
	    //维护管道
	    idx = i;
	    break;
	}
    }
}
template <typename T>
void ProcessPool<T>::setupSigPipe() {
    /*
     * 创建epoll事件监听表和信号管道
     * 设置信号除俩函数
     */
}

template <typename T>
void ProcessPool<T>::run() {
    if (idx != -1) {
	runChild();
	return ;
    }
    runParent();
}

template <typename T>
void ProcessPool<T>::runChild() {
    /*
     *每个子进程通过其在进程池中的序号值idx找到与父进程的通信管道
     * 子进程需要监听管道文件描述符pipeFd， 因为父进程通过它来通知子进程accept新连接
     */
    while (!stop) {
	
    }
}
template <typename T>
void ProcessPool<T>::runParent() {
    
}

} // end of namesapce zpt_ser

#endif
