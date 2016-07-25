#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>

#include "toolsBase.h"

namespace zpt_ser {

#define DEBUGEPOLL(x) { coutDebugMsg("tools", "error: %s", x); \
    _initState = Failure; \
    return \
}
#define DEBUGMSG(x) coutDebugMsg("tools", "%d closed\n", x)
#define DEBUGINFO(x) coutDebugMsg("tools", "%s", x)

class EPOLL:public ToolsBase {
private:
    static const int MAX_EVENT_NUMBER = 10000;

private:
    int _epollfd; //文件描述符
    struct epoll_event _ev, _events[MAX_EVENTS]; //事件
    unsigned int curEpollSize; //当前Epoll监听用户数目
    bool _isRun;


public:
   EPOLL();
   ~EPOLL(); 
   int addFd(int);  //向epoll中添加描述符int
   void removeFd(int); //从epoll中删除描述符int
   void stop() { _isRun = false; }
   void run() { _isRun = true; }
   int run(int (*handle)(int)); //开始执行
   
private:
    int setnonblocking(int sockfd);  //设置非阻塞

};
//SOCKLEN

EPOLL::EPOLL():_initState(Successful):curEpollSize(0), _isRun(flase) {
    _epollfd = epoll_create(5);
    if (_epollfd == -1) {
	DEBUGEPOOL("epoll_create failure");
    }
}

EPOLL::~EPOLL() {
    close(_epollfd);
}

int EPOLL::run(int (*handle)(int)) {
    int nfds;
    while (true) {
	nfds = epoll_wait(_epollfd, _events, MAX_EVENT_NUMBER, -1);
	if (nfds == -1) {
	    _err = "epoll_pwait";
	    return -1; //需要修改，
	}
	for (int n=0; n<nfds; n++) {
	   if ((_events[n].events & EPOLLERR) || (_events[n].events & EPOLLHUP)) {
		_err = "epoll error";
		cout << _err << endl;
		remove(_events[n].data.fd);
		close(_events[n].data.fd);
		std::cout << "close " << _events[n].data.fd << std::endl;
		continue;
	    }
	    if (_events[n].events & EPOLLIN) {
		if (handle(_events[n].data.fd) == 0) {
		    _err = "handle error ";
		    cout << _err << endl;
		    close(_events[n].data.fd);
		    std::cout << "close " << _events[n].data.fd << std::endl;

		}
	    }
	}
    }    
}

//添加文件描述符
int EPOLL::addFd(int fd) {
   setnonblocking(fd);
   _ev.events = EPOLLIN | EPOLLET;
   _ev.data.fd = fd;
   if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &_ev) == -1) {
	DEBUGEPOLL("epoll_ctl: conn_sock");
	return 0;
    }
    DEBUGINFO("connected success");
    return 1;
}

//删除文件描述符
void EPOLL::removeFd(int fd) {
    epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    DEBUGMSG(fd);
}

//将文件描述符设置为非阻塞
int EPOLL::setnonblocking(int sockfd) {
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK) == -1)
	return -1;
    return 0;
}


}//end of namespace zpt_frame

#endif
