#ifndef SEMCOMMON_H_
#define	SEMCOMMON_H_

#include "../communicateBase.h"

namespace zpt_frame {
   
//MesType没有实际意义
template <typename MesType, unsigned int Size, CurOwn own>
class BaseSem:public BaseCommunicate<MesType, Size, own> {
public:
    virtual ~BaseSem() {}
    virtual bool semP(short, int) = 0;
    virtual bool semV(short, int) = 0;
};
    
} //end of namespace zpt_frame

#endif
