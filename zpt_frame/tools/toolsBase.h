#ifndef TOOLS_BASE_H_
#define TOOLS_BASE_H_

#include "common.h"

namespace zpt_frame {

enum InitState { Successful, Failure}; //模块初始化状态，成功/失败
enum CurOwn { Leader, Follower};

//工具类的基类
class ToolsBase {
public:
    InitState _initState;

public:
    ToolsBase():_initState(Successful) {}
    virtual ~ToolsBase() {}
};

};

#endif
