#ifndef COMMUNICATE_BASE_H_
#define COMMUNICATE_BASE_H_

/*
 * 每个IPC工具都有唯一的ID值来标识，在创建IPC工具时需要首先确定要创建的IPC工具的ID值。通过访问key来访问IPC工具
 */

#include "../toolsBase.h"

namespace zpt_frame {


//通信的基类
template <typename MesType, unsigned int Size, CurOwn own>
class BaseCommunicate:public ToolsBase {
public:
    CurOwn _own;

public:
    BaseCommunicate():_own(own) {}
    virtual ~BaseCommunicate() {};
};

} //end_of namespace zpt_frame

#endif // COMMUNICATE_BASE_H_
