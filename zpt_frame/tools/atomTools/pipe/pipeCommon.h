#ifndef PIPECOMMON_H_
#define PIPECOMMON_H_


#include "../common.h"

namespace zpt_ser {

#define FIFONAME "fifo"  //测试所用

enum PipeCurOwn {NoSet, Server, Client};//管道的当前用户的身份
enum CommunicateType {Consanguinity, NoConsanguinity}; // 管道对象类型，为亲缘或者非亲缘，后者对不存在管道不删除
enum PipeType {Named, NoName}; //管道类型：有名管道，


template <typename T>
class BasePipe : public BaseCommunicate {
public:
    BasePipe() {}
    virtual ~BasePipe() {}
    virtual bool writeData(T buf, unsigned int bufLen) = 0;
    virtual bool readData(T buf, unsigned int bufLen) = 0;
}; //end of class BasePipe


} //end_of namespace zpt_ser

#endif // COMMON_H_
