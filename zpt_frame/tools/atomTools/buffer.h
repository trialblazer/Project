#ifndef BUFFER_H__
#define BUFFER_H__

/*
 * 该文件实现一个缓冲区，使用链表，节点为BufNode，
 * 只维护动态申请的内存空间的地址，不对申请的空间有释放操作
 */

#include "../common.h"
#include "./sem/locker.h"
#include <stdlib.h>

namespace  zpt_frame {

class BufNode {
public:
    void * buf; //存储的信息
    int size;  //存储信息大小
    BufNode* next;

public:
    BufNode(void* b, int s):buf(b), size(s), next(NULL) {}
};


class Buffer {
private:
    BufNode* head;  //缓冲区的头
    BufNode* tail;   //缓冲区的尾
    P_Locker _locker;  //缓冲区的锁

    
private:
    void freeBuffer(); //释放缓冲区，但是不释放缓冲区中指向的空间

public:
    Buffer():head(NULL), tail(NULL) {}
    ~Buffer() { freeBuffer(); }
    void* readBuf(int &size); //从缓冲区中拿一个数据，size返回大小，函数返回数据的指针
    bool writeBuf(void* buf, int size); //往缓冲区中写入一个数据，大小为size
    bool empty() { return head == NULL; } //判断缓冲区中是否有数据
};

void Buffer::freeBuffer() {
    while (_locker.lock() == false) ;
    BufNode* tmpt;
    while (head != NULL) {
	tmpt = head;
	head = head->next;
	free(tmpt);
    }
    head = NULL;
    tail = NULL;
    if (_locker.unlock() == false) {
	coutDebugMsg("tools/atomTools/buffer.h", "error:%s", "unlock error");
    }
}
void* Buffer::readBuf(int& size) {
    while (_locker.lock() == false) ;
    if (empty()) 
	return NULL;
    BufNode* tmp = head;
    size = head->size;
    head = head->next;
    if (NULL == head) {
	head = NULL;
	tail = NULL;
    }
    void* returnBuf = tmp->buf;
    delete tmp;
    if (_locker.unlock() == false) {
	coutDebugMsg("tools/atomTools/buffer.h", "error:%s", "unlock error");
    }
    return returnBuf;
}

bool Buffer::writeBuf(void* buf, int size) {
    while (_locker.lock() == false) ;
    BufNode* addNode = new BufNode(buf, size);
    if (addNode == NULL) {
	coutDebugMsg("tools/atomTools/buffer", "error: %s", "memory allocate failure");
	if (_locker.unlock() == false) {
	    coutDebugMsg("tools/atomTools/buffer.h", "error:%s", "unlock error");
	}
	return false;
    }
    if (head == NULL) {
	head = addNode;
	tail = head;
    } else {
        tail->next = addNode;
	tail = addNode;
    }
    if (_locker.unlock() == false) {
	coutDebugMsg("tools/atomTools/buffer.h", "error:%s", "unlock error");
    }
    return true;
}


} // end of namespace zpt_frame


#endif
