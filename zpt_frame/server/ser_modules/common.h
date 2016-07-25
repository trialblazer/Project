#ifndef MODULE_COMMON_H_
#define MODULE_COMMON_H_

#include "../common.h"
#include "../../tools/atomTools/time.h"
#include "../../tools/atomTools/pthread/thread.h"
#include "../../tools/toolModule/shmModule.h"

namespace zpt_frame {

#define MaxControlProcessSize 1
#define MaxConnectProcessSize 1
#define MaxListenProcessSize 4
#define MaxIOProcessSize 4
#define MaxWorkerProcessSize 4


enum ModuleState { Normal, Abnormal};   //模块初始化状态
enum ModuleCurState {Run, Stop };	//模块当前状态，运行/停止 

enum ListenModulePattern {LongCon, ShortCon}; //监听进程连接模式，长连接，短连接

class BaseModule {
public:
   ModuleState	    _mdState;
   ModuleCurState   _mdCurState;
   Time _time; //模块启动时间
   

public:
    BaseModule():_mdState(Normal), _mdCurState(Stop) {}
    virtual ~BaseModule() {}
    virtual void init() = 0;
    virtual void* run(void* f = NULL) = 0;
    virtual void stop() { _mdCurState = Stop; }
    virtual time_t getRunTime() {
	Time time;
	return time.getSecs()-_time.getSecs();
    }
    //virtual void restart() =  { init(); run(); }
};

} // end of namespace zpt_frame

#endif
