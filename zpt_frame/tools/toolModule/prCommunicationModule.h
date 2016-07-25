#ifndef PRCOMMUNICATION_H_
#define PRCOMMUNICATION_H_

#include "../atomTools/buffer.h"
//#include "../toolsBase.h"
#include "./shmModule.h"

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
 * 该类实现了各模块进程通信
 * 模板类参数为： 内部工具类型， 消息类型，模块所属进程类型
 */
template <typename ToolType, typename MesType, CurOwn own>
class Communicate:ToolsBase {
public:
    static const unsigned int BufSize = 1024; //最大消息长度

protected:
    ToolType*           _tol; 
    Buffer              _buf;       //缓冲区

public:
    Communicate(int proj_id);
    virtual ~Communicate();
    int sendMsg(MesType msg, unsigned int msgSize, short flg = IPC_NOWAIT);
    int recvMsg(MesType msg, unsigned int msgSize, short flg = IPC_NOWAIT);
    bool run();
};

template <typename ToolType, typename MesType, CurOwn own>
Communicate<ToolType, MesType, own>::Communicate(int proj_id) {
    _tol = new ToolType(proj_id);
    ASSERT(_tol);
}

template <typename ToolType, typename MesType, CurOwn own>
Communicate<ToolType, MesType, own>::~Communicate() {
    FREE(_tol);
}

template <typename ToolType, typename MesType, CurOwn own>
int Communicate<ToolType, MesType, own>::sendMsg(MesType msg, unsigned int msgSize, short flg) {
    int size;
    void* buf = NULL;
    int i = 0;
    while (_buf.empty() == false && i<100) { //每次先发送之前没有发送的消息
	buf = _buf.readBuf(size);
	while (i < 100) {
	    if (_tol->sendMsg(buf, size, flg) == true) {
		delete buf; //释放申请内存
		buf = NULL;
		break;
	    }
	   i++; 
	}
	if (i == 100) //尝试放送多次还是失败，暂不发送
	    break;
	else 
	    i = 0;
    }
    bool result =  _tol->sendMsg(msg, msgSize, flg);
    if (result == true)
	return result;
    result = _buf.writeBuf(msg, msgSize);
    if (result == false) {
	coutDebugMsg("tools/toolModule", "error: %s", "buf writeBuf");
    }
    return result;
}


template <typename ToolType, typename MesType, CurOwn own>
int Communicate<ToolType, MesType, own>::recvMsg(MesType msg, unsigned int msgSize, short flg) {
    return _tol->recvMsg(msg, msgSize, flg);
}

template <typename ToolType, typename MesType, CurOwn own>
bool Communicate<ToolType, MesType, own>::run() {
    return true;
}


} //end of namespace zpt_frame

#endif
