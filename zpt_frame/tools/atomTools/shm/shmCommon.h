#ifndef SHMCOMMON_H_
#define SHMCOMMON_H_

#include "../communicateBase.h"


namespace zpt_frame {

template <typename MesType, unsigned int size, CurOwn own>
class BaseShm : public BaseCommunicate<MesType, size, own> {
public:
    BaseShm() {} 
    virtual ~BaseShm() {}
    virtual bool writeData(MesType buf, unsigned int bufLen) = 0;
    virtual bool readData(MesType buf, unsigned int bufLen) = 0;
};

}// end of namespace zpt_ser


#endif
