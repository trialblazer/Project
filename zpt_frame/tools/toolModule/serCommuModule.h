#ifndef SERCOMMUMODULE_H_
#define SERCOMMUMODULE_H_

#include "./prCommunicateionModule.h"
#include <stdlib.h>

namespace zpt_frame {

enum Modules {connect, Listen, IoHandler, Worker, control}; //模块标识

template <Modules module, typename MesType>
class SerCommuModule {
public:
    #define Con_Ctl 1
    #define Con_Lst (Con_Ctl+MaxConnectProcessSize*MaxControlProcessSize*6) //最多有（Lst_Ctl-Con_lst)/6个监听进程
    #define Lst_Ctl (Con_Lst+MaxConnectProcessSize*MaxListenProcessSize*6)
    #define Lst_Io  (Lst_Ctl+MaxListenProcessSize*MaxControlProcessSize*6)
    #define Io_Ctl (Lst_Io+MaxListenProcessSize*MaxIOProcessSize*6) //最多有（Io_Ctl-Lst_Io)/6个IO进程
    #define Io_Wrk (Io_Ctl+MaxIOProcessSize*MaxControlProcessSize*6)
    #define Wrk_Ctl (Io_Wrk+MaxIOProcessSize*MaxWorkerProcessSize*6)
    #define ToolType(own) { ShmModule<MessType, Communicate::BufSize, own> }

private:
    ToolsBase* ctlCommu;
    ToolsBase** firstCommu;
    ToolsBase** secondCommu;

public:
    SerCommuModule();
    ~SerCommuModule();
    bool sendMsg(Modules moduleName, int index, MesType message, unsigned int size);
    bool recvMsg(Modules moduleName, int index, MesType message, unsigned int size);
};


template <Modules module, typename MesType>
SerCommuModule<module, MesType>::SerCommuModule() {
    ctlCommu = NULL;
    firstCommu = NULL;
    secondCommu = NULL;

//整个通信模块主要分三块，与总控进程通信，与下一进程进行通信， 与上一进程进行通信，如果没有上进程或下进程，则置0
    int firstCommuSize = 0; //下进程通信数目
    int secondCommuSize = 0;//上进程通信数目
    int firstProjId = 0;//与下进程通信id
    int secondProjId = 0;//与上进程通信id
    int thirdProjId = 0; //与总控进程通信id

    if (module == connect) { //如果时连接模块
	firstCommuSize = MaxListenProcessSize;
	thirdProjId = Con_Ctl;
	firstProjId = Con_Lst;
    } else if (module == Listen) {//如果是监听模块
	firstCommuSize = MaxIOProcessSize;
	secondCommuSize = maxConnectProcessSize;
	thirdProjId = Lst_Ctl;
	secondProjId = Con_Lst;
	firstProjId = Lst_Io;
    } else if (module == IoHandler) {//如果时读写模块
	firstCommuSize = MaxWorkerProcessSize;
	secondCommuSize = MaxListenProcessSize;
	thirdProjId = Io_Ctl;
	secondProjId = Lst_Io;
	firstProjId = Io_Wrk;
    } else if (module == Worker) {//如果是处理模块
	secondCommuSize = MaxIOProcessSize;
	thirdProjId = Con_Ctl;
	secondProjId = IO_Wrk;
    }
    if (module == Control) {
	ctlCommu = new communicate<ToolType(Leader), MesType, Leader> _commuControl();
    } else {
	ctlCommu = new communicate<ToolType(Follower), MesType, Follower> _commuControl();
	
    }
    //初始化firstCommu, secondCommu
    for (int i=0; i< ; i++) {
	firstCommu[i];
    }
}

template <Modules module, typename MesType>
SerCommuModule<module, MesType>::~SerCommuModule() {
//释放申请空间ctlCommu, firstCommu, secondCommu
}

template <Modules module, typename MesType>
bool SerCommuModule<module, MesType>::sendMsg(Modules moduleName, int index, MesType message, unsigned int size) {
    return true;
}

template <Modules module, typename MesType>
bool SerCommuModule<module, MesType>::recvMsg(Modules moduleName, int index, MesType message, unsigned int size) {
    return true;
}
} // end of namespace zpt_frame

#endif 
