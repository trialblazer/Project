/* 该类主要封装了无名管道，主要用于父子进程间通信
 * <limit.h> PIPE_BUF: 4096
 * 对于无名管道，pipedes[0]用于读，pipedes[1]用于写
 * 无名管道只能存在于进程通信期间
 * 如果试图向已经填满的管道中写，系统会自动阻塞，因此恢复时另一个write可能会抢现执行，就会出现交叉写入。每次写入数据小于PIPE_BUF
 * 一个管道不能同时被两个进程打开
 * 写入时可能会存在设备忙，如果O_NONBLOCK和O_NDELAY标识被清除，write将会被阻塞，否则当没有大于PIPE_BUF时不会阻塞，直接返回-1
 */
#ifndef NONAMEPIPE_H_
#define NONMAEPIPE_H_ 



#include "pipeCommon.h"
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <stdlib.h>

#define DEBUG


namespace zpt_ser {

//T为缓冲区类型，size为缓冲区大小
template <typename T, unsigned int size>
class NoNamePipe:public BasePipe<T> {
private:
    int pipedes[2];

public:
    NoNamePipe();
    ~NoNamePipe();
    bool writeData(T buf, unsigned bufLen);	//将buf中的数据写入管道
    bool readData(T buf, unsigned bufLen);		//将管道中的数据读入buf中
}; 

template <typename T, unsigned int size>
NoNamePipe<T, size>::NoNamePipe() {
    using namespace std;
    if (pipe(pipedes) == -1) {
	cerr << "create noNamePipe error" << endl;
	exit(EXIT_FAILURE);
    }    
}    

template <typename T, unsigned int size>
NoNamePipe<T, size>::~NoNamePipe() {}

template <typename T, unsigned int size>
bool NoNamePipe<T, size>::writeData(T buf, unsigned bufLen) {
    #ifdef DEBUG
    using namespace std;
    #endif
    if (bufLen > size) {
	#ifdef DEBUG
	cerr << "write error, buflen is bigger than size" << endl;
	cout << "buf_len: " << bufLen << endl;
	cout << "buf_size: " << size << endl;
	#endif
	return false;
    }
    int len = write(pipedes[1], buf, bufLen);
    if (len == -1) { 
	#ifdef DEBUG
	cerr << "write error" << endl;
	cout << "write buf: " << buf << endl;
	cout << "read_buf size: " << size << endl;
	#endif
	return false;
    }
    #ifdef DEBUG
    cout << "write buf: " << buf << endl;
    cout << "read_buf size: " << size << endl;
    cout << "write len: " << len << endl;
    #endif
    return true;
}

template <typename T, unsigned int size>
bool NoNamePipe<T, size>::readData(T buf, unsigned bufLen) {
    #ifdef DEBUG
    using namespace std;
    #endif
    if (bufLen > size) {
	#ifdef DEBUG
	cerr << "read error, buflen is bigger than size" << endl;
	cout << "buflen: " << bufLen << endl;
	cout << "read_buf size: " << size << endl;
	#endif
	return false;
    }
    int len = read(pipedes[0], buf, size);
    if (len == -1) {
	#ifdef DEBUG
	cerr << "read error" << endl;
	cout << "read buf: " << buf << endl;
	cout << "read_buf size: " << size << endl;
	#endif
	return false;
    }
    #ifdef DEBUG
    cout << "read buf: " << buf << endl;
    cout << "read_buf size: " << size << endl;
    cout << "read len: " << len << endl;
    #endif
    return true;
}

} //end_of namespace zpt_ser

#endif
