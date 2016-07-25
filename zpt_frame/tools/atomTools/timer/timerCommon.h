#ifndef TIMERCOMMON_H_
#define TIMERCOMMON_H_
    
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

namespace zpt_ser {

#define BUFFSIZE 64

struct ClientData;

class BaseTimerNode {
public:
    BaseTimerNode() {}
    virtual ~BaseTimerNode() {}
};

class BaseTimer {
public:
    BaseTimer() {};
    virtual ~BaseTimer() {};
    virtual void addTimer(BaseTimerNode*) = 0;
    virtual void delTimer(BaseTimerNode*) = 0;
    virtual void tick() = 0;
};

/*
struct ClientData { //用户数据结构
    sockaddr_in address;
    int sockfd;
    char buf[BUFFSIZE];
    BaseTimer* timer;
};
*/

}//end of namespace zpt_ser

#endif
