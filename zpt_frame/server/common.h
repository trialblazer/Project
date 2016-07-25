#ifndef SERVER_COMMON_H_
#define SERVER_COMMON_H_


#include <string.h>

#include "../debug/debug.h"
#include "config.h"

namespace zpt_frame {

//对普通用户使用短连接，登录用户使用长连接，构成混合连接
enum FrameType {LongConnection, ShortConnection, MixConnection}; //系统类型：长连接，短连接,混合连接

enum MsgType {Ordinary}; // 内部消息类型,一般数据
class Message {  // 内部传送消息类
private:
    MsgType _msgType; //消息类型
    char data[0]; //消息

public:
    Message(MsgType _mt, void* msg, int size) {
	_msgType = _mt;
       memcpy(data, msg, size);
    }
    void* getData() { return data; }
    MsgType getMsgType() { return _msgType; }
    int getSize() { return sizeof(data)+sizeof(MsgType);     }
};

} // end of namespace zpt_frame
#endif
