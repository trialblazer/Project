#ifndef WHEELTIMER_H_
#define WHEELTIMER_H_

#include "timerCommon.h"
#include <stdlib.h>

namespace zpt_ser {

class TwTimer:public BaseTimerNode {
public:
    TwTimer(int rot, int ts):next(NULL), prev(NULL), rotation(rot), timerSlot(ts) {}
    virtual ~TwTimer();

public:
    int rotation; //记录定时器在时间轮转多少圈后生效
    int timerSlot; //记录定时器属于时间轮上哪个槽
    void (*cb_func)(ClientData*); //定时器回调函数
    ClientData* userData;
    TwTimer* next;
    TwTimer* prev;
};//end of class WheelTimer

class TimerWheel:public BaseTimer {
private:
    static const int N = 60; //时间轮槽上的数目
    static const int SI = 1; //每1S时间轮转动一次，
    TwTimer* slots[N]; //时间轮的槽，其中每个元素指向一个定时器链表，无序
    int curSlot;  //时间轮当前槽

public:
    TimerWheel();
    ~TimerWheel();
    TwTimer* addTimer(int timeout);
    virtual void delTimer(TwTimer* timer);
    virtual void tick();
};//end of class TimerWheel

TimerWheel::TimerWheel():curSlot(0) {
    for (int i=0; i<N; i++) 
	slots[i] = NULL; //初始化头结点
}

TimerWheel::~TimerWheel() {
    for (int i=0; i<N; i++) { //遍历每个槽，并销毁里面的计时器
	TwTimer* tmp = slots[i];
	while (tmp) {
	    slots[i] = tmp->next;
	    delete tmp;
	    tmp = slots[i];
	}
    }
}

TwTimer* TimerWheel::addTimer(int timeout) {
    if (timeout < 0)
	return NULL;
    int ticks = 0;
    ticks = (timeout < SI? 1:(timeout/SI));
    int rotation = ticks/N;//计算定时器在时间轮转动多少圈被触发
    int ts = (curSlot + (ticks%N))%N; //计算定时器应该被插入哪个槽中
    TwTimer* timer = new TwTimer(rotation, ts);
    if (timer == NULL) {
	#ifdef DEBUG
	std::cout << "memory allocate fialure" << std::endl;
	#endif
	return NULL;
    }
    if (!slots[ts]) {
	slots[ts] = timer;
    } else {
	timer->next = slots[ts];
	slots[ts]->prev = timer;
	slots[ts] = timer;
    }
    return timer;
}

void TimerWheel::delTimer(TwTimer* timer) {
    if (timer == NULL)
	return ;
    int ts = timer->timerSlot;
    if (timer == slots[ts]) {
	slots[ts] = slots[ts]->next;
	if (slots[ts]) {
	    slots[ts]->prev = NULL;
	}
	delete timer;
    } else {
	timer->prev->next = timer->next;
	if (timer->next)
	    timer->next->prev = timer->prev;
	delete timer;
    }
}

void TimerWheel::tick() {
    TwTimer* tmp = slots[curSlot];
    while (tmp) {
	if (tmp->rotation > 0) {
	    tmp->rotation--;
	    tmp = tmp->next;
	} else {
	    tmp->cb_func(tmp->userData);
	    if (tmp == slots[curSlot]) {
		slots[curSlot] = tmp->next;
		delete tmp;
		if (slots[curSlot])
		    slots[curSlot]->prev = NULL;
		tmp = slots[curSlot];
	    } else {
		tmp->prev->next = tmp->next;
		if (tmp->next)
		    tmp->next->prev = tmp->prev;
		TwTimer* tmp2 = tmp->next;
		delete tmp;
		tmp = tmp2;
	    }
	}
    }
    curSlot = ++curSlot%N;
}

}// end of namespace zpt_ser

#endif
