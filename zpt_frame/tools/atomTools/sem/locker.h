#ifndef LOCKER_H_
#define LOCKER_H_

#include <exception>
#include <pthread.h>
#include <semaphore.h>

namespace zpt_frame { 


//封装POSIX信号量的类
class P_Sem {
private:
    sem_t m_sem;

public:
    P_Sem() {//初始化信号量
	if (sem_init(&m_sem, 0, 0) != 0) {
	    throw std::exception();
	}
    }
    ~P_Sem() {//销毁线号量
	sem_destroy(&m_sem);
    }
    bool wait() {//将信号量的值减1
	return sem_wait(&m_sem) == 0;
    }
    bool post() {//将信号量的值加1
	return sem_post(&m_sem) == 0;
    }
};

//封装互斥锁的类
class P_Locker {
private:
    pthread_mutex_t m_mutex;

public:
    P_Locker() { //初始化互斥锁
	if (pthread_mutex_init(&m_mutex, NULL) != 0) {
	    throw std::exception();
	}
    }
    ~P_Locker() {//销毁互斥锁
	pthread_mutex_destroy(&m_mutex);
    }
    bool lock() { //给一个互斥锁枷锁
	return pthread_mutex_lock(&m_mutex) == 0;
    }
    bool unlock() {
	return pthread_mutex_unlock(&m_mutex) == 0;
    }
};

//封装条件变量
class P_Cond {
private:
   pthread_mutex_t m_mutex;
   pthread_cond_t m_cond;

public:
    P_Cond() {//创建并初始化条件变量
	if (pthread_mutex_init(&m_mutex, NULL) != 0) {
	    throw std::exception();
	}
	if (pthread_cond_init(&m_cond, NULL) != 0) {
	    pthread_mutex_destroy(&m_mutex);
	    throw std::exception();
	}
    }
    ~P_Cond() {//销毁条件变量
	pthread_mutex_destroy(&m_mutex);
	pthread_cond_destroy(&m_cond);
    }
    bool wait() {//等待条件变量
	int ret = 0;
	pthread_mutex_lock(&m_mutex);
	ret = pthread_cond_wait(&m_cond, &m_mutex);
	pthread_mutex_unlock(&m_mutex);
	return ret == 0;
    }
    bool signal() {//唤醒等待条件变量的线程
	return pthread_cond_signal(&m_cond) == 0;
    }
}; 





}//end of namespcace zpt_frame

#endif
