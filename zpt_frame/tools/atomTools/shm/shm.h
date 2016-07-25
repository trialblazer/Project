#ifndef SHM_H_
#define SHM_H_


/*
 * 共享内存就是分配一块能被其他进程访问的内存，由shmid_ds管理
 *
 */
#include "shmCommon.h"
#include <sys/shm.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

using namespace std;


#define DEBUGSHM_
#define DEBUGSHM(x) coutDebugMsg("atomTools", "error: %s", x);

namespace zpt_frame {

//规定Leader为写端析构时销毁共享内存，Follower为读端，
template <typename MesType, unsigned int size, CurOwn own>
class Shm:public BaseShm<MesType, size, own> {
protected:
    int		_shmid;
    void*	_shmaddr;

public:
    Shm(int proj_id);
    virtual ~Shm();
    virtual bool writeData(MesType buf, unsigned int bufLen);
    virtual bool readData(MesType buf, unsigned int bufLen);
}; //end of class Shm


template <typename MesType, unsigned int size, CurOwn own>
Shm<MesType, size, own>::Shm(int proj_id) {
    key_t shmkey = ftok(".", proj_id);
    if (shmkey == -1) {
	DEBUGSHM("ftok failure");
	this->_initState = Failure; //this使用，是因为模板类继承
	return ;
    }
    _shmid = shmget(shmkey, size, IPC_CREAT | 0666); //如果不存在就创建 
    if (_shmid == -1) { //创建或打开失败
	DEBUGSHM("shmget call failed");
	this->_initState = Failure;
	return ;
    }
    _shmaddr = shmat(_shmid, NULL, 0); //将共享内存附加到进程内存区
    if (_shmaddr == NULL) {
	this->_initState = Failure;
	DEBUGSHM("failed to create shared memory segment");
	if (shmctl(_shmid, IPC_RMID, NULL) == -1) //删除共享内存
	    DEBUGSHM("failed to remove memory segment");
	return ;
    }
}

template <typename MesType, unsigned int size, CurOwn own>
Shm<MesType, size, own>::~Shm() {
    using namespace std;
    if (own == Leader) {
	if (shmctl(_shmid, IPC_RMID, NULL) == -1) { //删除共享内存
	    DEBUGSHM("failed to remove memory segment" );
	    this->_initState = Failure;
	    return ;
	}
    }
}

template <typename MesType, unsigned int size, CurOwn own>
bool Shm<MesType, size, own>::writeData(MesType buf, unsigned int bufLen) {
    if (bufLen > size) {
	#ifdef DEBUGSHM_
	cout << "bufLen is bigger than the max size"  << endl;
	cout << "bufLen: " << bufLen << endl;
	cout << "maxSize: " << size << endl;
	cout << "buf: " << buf << endl;
	#endif
	return false;
    }
    memcpy(_shmaddr, buf, bufLen);
    #ifdef DEBUGSHM_
    cout << "write success" << endl;
    cout << "bufLen: " << bufLen << endl;
    cout << "maxSize: " << size << endl;
    cout << "buf: " << buf << endl;
    #endif

    return true;
}

template <typename MesType, unsigned int size, CurOwn own>
bool Shm<MesType, size, own>::readData(MesType buf, unsigned int bufLen) {
    #ifdef DEBUGSHM_
    using namespace std;
    #endif
    memcpy(buf, _shmaddr, bufLen);
    memset(_shmaddr, '\0', size);
    #ifdef DEBUGSHM_
    cout << "read success" << endl;
    cout << "bufLen: " << bufLen << endl;
    cout << "maxSize: " << size << endl;
    cout << "buf: " << buf << endl;
    #endif
    return true; 
}

}// end of namespace zpt_ser

#endif
