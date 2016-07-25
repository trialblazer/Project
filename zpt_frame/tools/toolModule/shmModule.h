#ifndef SHMMODULE_H_
#define SHMMODULE_H_

#include "../atomTools/shm/shm.h"
#include "../atomTools/sem/sem.h"
#include "../atomTools/shm/shmCommon.h"

#define ASSERT(x)  { \
    if (x == NULL) {\
	coutDebugMsg("toolModule", "error: %s", "memory allocate failure"); \
	this->_initState = Failure; \
	return ;\
    }    \
}
#define FREE(x)  { \
    if (x!=NULL) \
	delete x;\
    }



namespace zpt_frame {

/*
 * 该类为shm的封装组合类，添加了同步机制
 */
template <typename MesType, unsigned int size, CurOwn own>
class ShmModule:public BaseCommunicate<MesType, size, own> {
protected:
    Shm<MesType, size, own>* _wShm; //写共享内存
    Shm<MesType, size, own>* _rShm; //读共享内存
    Sem<own>*	_semLW;	    //写同步信号量
    Sem<own>*	_semLR;	    //读同步信号量
    Sem<own>*   _semFW;
    Sem<own>*   _semFR;

public:
    ShmModule(int proj_id);
    virtual ~ShmModule();
    int sendMsg(MesType msg, unsigned int msgSize, short flg = IPC_NOWAIT); //默认使用操作失败直接返回
    int recvMsg(MesType msg, unsigned int msgSize, short flg = IPC_NOWAIT); //默认使用操作失败直接返回

};

template <typename MesType, unsigned int size, CurOwn own>
ShmModule<MesType, size, own>::ShmModule(int proj_id) {
    _wShm = new Shm<MesType, size, own>(proj_id);
    ASSERT(_wShm);
    _rShm = new Shm<MesType, size, own>(proj_id+1);
    ASSERT(_rShm);
    
    //创建四个信号量，分别作为Leader写共享内存区的读写标志，和Follower写内存共享区的读写标志
    _semLW = new Sem<own>(proj_id+2); 
    ASSERT(_semLW);
    _semLR = new Sem<own>(proj_id+3);
    ASSERT(_semLR);
    _semFW = new Sem<own>(proj_id+4);
    ASSERT(_semFW);
    _semFR = new Sem<own>(proj_id+5);
    ASSERT(_semFR);

    if (own == Leader) {
	_semLW->setVal(1); //Leader can wirte, Follower can't read
	_semLR->setVal(0); //Leader can't read, Follower can write
	_semFW->setVal(1); //Follower can write , Leader can't read
	_semFR->setVal(0); //Follower can't read, Leader can write
    } 
}

template <typename MesType, unsigned int size, CurOwn own>
ShmModule<MesType, size, own>::~ShmModule() {
    FREE(_wShm);
    FREE(_rShm);
    FREE(_semLW);
    FREE(_semLR);
    FREE(_semFW);
    FREE(_semFR);
}

template <typename MesType, unsigned int size, CurOwn own>
int ShmModule<MesType, size, own>::sendMsg(MesType msg, unsigned int msgSize, short flg) {
    int s = 0;
    if (own == Leader) { //如果是Leader则查看信号量semW
	if (_semLW->semP(flg) == false)
	    return s;
	s =  _wShm->writeData(msg, msgSize);
	_semFR->semV(flg); //Leader after write , Folower can read
    } else { //是Follower则查看线号两semR
	if (false == _semFW->semP(flg))
	    return s;
	s = _rShm->writeData(msg, msgSize);
	_semLR->semV(flg); //Follower after write, Leader can read
    }
    return s;
}


template <typename MesType, unsigned int size, CurOwn own>
int ShmModule<MesType, size, own>::recvMsg(MesType msg, unsigned int msgSize, short flg) {
    int s = 0;
    if (own == Leader) { //如果是Leader则查看信号量semW
	if (false == _semLR->semP(flg))
	    return s;
	s =  _rShm->readData(msg, msgSize);
	_semFW->semV(flg); //Leader after read , Follower can write
    } else { //是Follower则查看线号两semR
	if (false == _semFR->semP(flg))
	    return s;
	s = _wShm->writeData(msg, msgSize);
	_semLW->semV(flg); //Follower after read, Leader can Write
    }
    return s;
}


} //end of namespace zpt_frame

#endif
