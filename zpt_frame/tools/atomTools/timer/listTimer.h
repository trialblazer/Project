/*
 * 基于升序链表的定时器
 * 定时器包括：超时时间，任务回调函数
 * 添加定时器：O(n)
 * 删除定时器: O(1)
 * 执行任务：O(1)
 */ 

#ifndef LIST_TIMER_
#define LIST_TIMER_

#include <time.h>
#include "timerCommon.h"

namespace zpt_ser {


class TlTimer:public BaseTimerNode { //定时器类
public:
    TlTimer():prev(NULL), next(NULL) {}
	
public:
    time_t expire; //任务超时时间，使用绝对时间
    void (*cb_function)(ClientData*); //任务回调函数
    ClientData* userData; //回调函数处理的客户数据，由定时器的执行者传递给回调函数
    TlTimer* prev; //指向前一个定时器
    TlTimer* next; //指向后一个定时器
};

class TimerList:public BaseTimer { //定时器链表，是一个升序，双向链表
    TlTimer* head;
    TlTimer* tail;

private:
    virtual void addTimer(TlTimer* timer, TlTimer* list_head); 

public:
    TimerList():head(NULL), tail(NULL) {}
    virtual ~TimerList();
    virtual void addTimer(TlTimer* timer);
    virtual void adjustTimer(TlTimer* timer);
    virtual void delTimer(TlTimer* timer);
    virtual void tick();
}; // end of class SortTimerList

TimerList::~TimerList() {
    TlTimer* tmp = head;
    while (tmp) {//删除里面所有定时器
	head = tmp->next;
	delete tmp;
	tmp = head;
    }
}

void TimerList::addTimer(TlTimer* timer) { //添加一个定时器
    if (!timer)
	return ;
    if (!head) { //之前链表中没有定时器
	head = tail = timer;
	return ;
    }
    if (timer->expire < head->expire) { //timer定时器超时时间最短，添加到链表头部
	timer->next = head;
	head->prev = timer;
	head = timer;
	return ;
    }
    addTimer(timer, head); //把定时器插入到合适的位置，保证升序性
}

void TimerList::adjustTimer(TlTimer* timer) { //当某个定时器改变时，改变其位置，只考虑定时器延长
    if (!timer)
	return ;
    TlTimer* tmp = timer->next;
    if (!tmp || (timer->expire < tmp->expire)) {
	return ;
    }
    if (timer == head) { //先删除然后再插入
	head = head->next;
	head->prev = NULL;
	timer->next = NULL;
	addTimer(timer, head);
    } else {
	timer->prev->next = timer->next;
	timer->next->prev = timer->prev;
	addTimer(timer, timer->next);
    }
}

void TimerList::delTimer(TlTimer* timer) { //删除一个定时器
    if (!timer)
	return ;
    if ((timer == head) && (timer == tail)) {
	delete timer;
	head = NULL;
	tail = NULL;
	return ;
    }
    if (timer == head) {
	head = head->next;
	head->prev = NULL;
	delete timer;
	return ;
    }
    if (timer == tail) {
	tail = tail->prev;
	tail->next = NULL;
	delete timer;
	return ;
    }
    timer->prev->next = timer->next;
    timer->next->prev = timer->prev;
    delete timer;
}

void TimerList::tick() {
    if (!head)
	return ;
    #ifdef DEBUG
    using namespace std;
    cout << "timer tick" << endl;
    #endif
    time_t cur = time(NULL); //获取当前时间
    TlTimer* tmp = head;
    while (tmp) { //从头结点开始处理每一个定时器，直到遇到一个尚未指定的定时器
	if (cur < tmp->expire) {
	    break;
	}
	tmp->cb_function(tmp->userData);
	head = tmp->next;
	if (head) {
	    head->prev = NULL;
	}
	delete tmp;
	tmp = head;
    }
}

void TimerList::addTimer(TlTimer* timer, TlTimer* list_head) { //将timer添加到list_head之后的部分链表中，
    TlTimer* prev = list_head;
    TlTimer* tmp = prev->next;
    while (tmp) {
	if (timer->expire < tmp->expire) {
	    prev->next = timer;
	    timer->next = tmp;
	    tmp->prev = timer;
	    timer->prev = prev;
	    break;
	}
	prev = tmp;
	tmp = tmp->next;
    }
    if (!tmp) { //遍历完仍未找到大于目标定时器的超时时间，将目标定时器设为尾部
	prev->next = timer;
	timer->prev = prev;
	timer->next = NULL;
	tail = timer;
    }
}



} // end of namespace zpt_ser

#endif
