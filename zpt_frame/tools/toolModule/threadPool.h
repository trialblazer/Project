#ifndef THREADPOOL_H_
#define THREADPOOL_H_

/*
 * 使用pthread_create函数时，该函数的第三个参数必须指向一个静态函数。而要在一个静态函数中调用类的动态成员，只能通过：
    1. 通过类的静态对像来调用
    2. 将类的对象传递给静态函数
 */
#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
#include "../sem/locker.h"


namespace zpt_ser {

template<typename T>
class ThreadPool {
private:
    int		    pThreadNum;	    //线程池中的线程数
    int		    pMaxRequests;   //请求队列中允许的最大请求数
    pthread_t*	    pThread;	    //描述线程池的数组
    std::list<T*>   pWorkQueue;	    //请求队列
    P_Locker	    pQueueLocker;   //保护请求队列的互斥锁
    P_Sem	    pQueueStat;	    //是否有任务需要处理
    bool	    pStop;	    //是否结束线程

private:
    //工作线程运行的函数，他不断从工作队列中取出任务并执行
    static void* worker(void* arg);
    void run();

public:
    ThreadPool(int threadNumber=8, int maxRequest=10000);
    ~ThreadPool();
    bool append(T* request);
};


template<typename T>
ThreadPool<T>::ThreadPool(int threadNumber, int maxRequest) {
    pThreadNum = threadNumber;
    pMaxRequests = maxRequest;
    pStop = false;
    pThread = NULL;

    if ((pThreadNum <= 0) || (pMaxRequests <= 0)) {
	throw std::exception();
    }
    pThread = new pthread_t[pThreadNum];
    if (pThread == NULL) {
	throw std::exception();
    }
    //创建若干个线程，并将它们设置为脱离线程
    for (int i=0; i<pThreadNum; i++) {
	if (pthread_create(pThread+i, NULL, worker, this) != 0) {
	    delete [] pThread;
	    throw std::exception();
	}
	if (pthread_detach(pThread[i])) {
	    delete [] pThread;
	    throw std::exception();
	}
    }
}


template <typename T>
ThreadPool<T>::~ThreadPool() {
    delete [] pThread;
    pStop = true;
}


template <typename T>
bool ThreadPool<T>::append(T* request) {
    pQueueLocker.lock();
    if (pWorkQueue.size() > pMaxRequests) {
	pQueueLocker.unlock();
	return false;
    }
    pWorkQueue.push_back(request);
    pQueueLocker.unlock();
    pQueueStat.post();
    return true;
}


template <typename T>
void* ThreadPool<T>::worker(void* arg) {
    ThreadPool* pool = (ThreadPool*) arg;
    pool->run();
    return pool;
}

template <typename T>
void ThreadPool<T>::run() {
    while (!pStop) {
	pQueueStat.wait();
	pQueueLocker.lock();
	if (pWorkQueue.empty()) {
	    pQueueLocker.unlock();
	    continue;
	}
	T* request = pWorkQueue.front();
	pWorkQueue.pop_front();
	pQueueLocker.unlock();
	if (!request) 
	    continue;
	request->process();
    }
}

} //end of namespace zpt_ser

#endif
