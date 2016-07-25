#ifndef ZPT_FRAME_TIMER_H_
#define ZPT_FRAME_TIMER_H_

#include <time.h>

namespace zpt_frame {

class Time {
    time_t timep;
    
public:
    Time() {}
    char* getTime() {
	time(&timep);
	return asctime(gmtime(&timep));
    }
    time_t getSecs() {
	time(&timep);
	return timep;
    }
    friend operator ==(Time& t) {
	return getSecs() == t.getSecs();
    }
};

}

#endif
