#ifndef	ZPTSEM_H_
#define ZPTSEM_H_
/*
 * 信号量是一个计数器，常用于对临界资源的访问的同步
 * 信号量大于0时，表示可供并发进程使用的资源，小于0标识等待是哟那个临界资源的进程数
 */
#include "semCommon.h"
#include <sys/sem.h>
#include <sys/ipc.h>


#define DEBUGMSG(x) coutDebugMsg("atomTools", "error: %s", x);

namespace zpt_frame {

template <CurOwn own, unsigned int nsems=1, typename T=int>
class Sem:BaseSem<T, nsems, own> {
private:
    int	_semid;
    
private:
    bool semPV(bool isP, int index, short flg);

public:
    Sem(int proj_id); 
    virtual ~Sem();
    bool semV(short flg = SEM_UNDO, int index=0);  
    bool semP(short flg = SEM_UNDO, int index=0);
    bool setVal(int value) {
	union semun {
	    int val;
	    struct semid_ds *buf;
	    unsigned short* array;
	} _semun;
	_semun.val = value;
	int ret = semctl(_semid, 0, SETVAL, _semun);
	if (ret == -1) {
	    DEBUGMSG("set sem Value error");
	    return false;
	}
	return true;
    }
    int getVal(void) {
	int res;
	if ((res=semctl(_semid, 0, GETVAL)) == -1) {
	    DEBUGMSG("semctl error");
	    return -1;
	}
	return res;
    }
};

template <CurOwn own, unsigned int nsems, typename T>
Sem<own, nsems, T>::Sem(int proj_id) {
    key_t key = ftok(".", proj_id);
    if (key == -1) {
	DEBUGMSG("ftok error");
	this->_initState = Failure;
	return ;
    }
    if (this->_own == Leader) {
	_semid = semget(key, nsems, IPC_CREAT | 0666); //创建信号量集合，由Leader创建，并且初始化
    } else if (this->_own == Follower) { 
	_semid = semget(key, 0, IPC_CREAT | 0666); //打开信号量集合，由Follower打开
    }
    if (_semid == -1) {
	DEBUGMSG("semget error");
	this->_initState = Failure;
    }
}

template <CurOwn own, unsigned int nsems, typename T>
Sem<own, nsems, T>::~Sem() {
    if (own == Leader)
	semctl(_semid, 0, IPC_RMID, (struct msguid_da*)0); //删除信号量集合
}


template <CurOwn own, unsigned int nsems, typename T>
bool Sem<own, nsems, T>::semPV(bool isP, int index, short flg) {
    if (index <0) {
	DEBUGMSG("index of array cannot equals a minus value?");
	return false;
    }
    struct sembuf buf;
    buf.sem_num = index;
//    buf.sem_flg = SEM_UNDO;
//    buf.sem_flg = IPC_NOWAIT;
    buf.sem_flg = flg;
    if (isP) {
	buf.sem_op = -1;
    } else {
	buf.sem_op = 1;
    }
    if (semop(_semid, &buf, 1) == -1) {
	if (buf.sem_flg != IPC_NOWAIT)
	    DEBUGMSG("a wrong operation to semaphore occurred!");
	return false;
    }
    return true; 
}

template <CurOwn own, unsigned int nsems, typename T>
bool Sem<own, nsems, T>::semP(short flg, int index) { //默认使用一直等待
    bool isP = true;
    return semPV(isP, index, flg);
}

template <CurOwn own, unsigned int nsems, typename T>
bool Sem<own, nsems, T>::semV(short flg, int index)  { //默认使用一直等待
    bool isP = false;
    return semPV(isP, index, flg);
}

} //end of namespace zpt_frame

#endif
