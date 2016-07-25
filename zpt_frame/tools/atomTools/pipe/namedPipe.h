#ifndef NAMEDPIPE_H_
#define NAMEDPIPE_H_

#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <string>
#include <fcntl.h>
#include <stdlib.h>
#include "pipeCommon.h"

namespace zpt_ser {

//如果时亲缘关系的父子进程间通信则默认就行，非亲缘关系进程通信构造函数第二个参数指定为NoConsanguinity

//T为传输数据类型，应该为指针，size为缓冲去打小
template <typename T, unsigned int size>
class NamedPipe:public BasePipe<T> {
private:
    std::string name;
    bool _type;
    
private:
    bool openW(int& fd); //以只写方式打开管道文件
    bool openR(int& fd); //fd保存打开文件描述符
    
public:
    NamedPipe(std::string name, CommunicateType t);
    ~NamedPipe();
    bool writeData(T buf, unsigned int bufLen);
    bool readData(T buf, unsigned int bufLen);
}; // end of class NamedPipe



template <typename T, unsigned int size>
NamedPipe<T, size>::NamedPipe(std::string n, CommunicateType t = Consanguinity) {
    _type = t;
    using namespace std;
    this->name = n;
    if (t == Consanguinity) {
        unlink(name.c_str());  //如果该文件存在，先删除该文件
    } else {
	if (access(name.c_str(), F_OK) != -1) { // 管道文件存在，直接返回
	    return ;
	}
    }
    if (mkfifo(name.c_str(), 0744) == -1) { //创建管道 
	cerr << "namedPipe create error" << endl;
	exit(1);
    }
}


template <typename T, unsigned int size>
NamedPipe<T, size>::~NamedPipe() {
    
}

template <typename T, unsigned int size>
bool NamedPipe<T, size>::openW(int& fd) {
    #ifdef DEBUG
    using namespace std;
    #endif
    if ((fd = open(name.c_str(), O_WRONLY)) == -1) {
	#ifdef DEBUG
	cerr << "openW: open pipeFile " << name << " error" << endl;
	#endif
	return false;
    }
    return true;
}

template <typename T, unsigned int size>
bool NamedPipe<T, size>::openR(int& fd) {
    #ifdef DEBUG
    using namespace std;
    #endif
    if ((fd = open(name.c_str(), O_RDONLY)) == -1) {
	#ifdef DEBUG
	cerr << "openR: open pipeFile " << name << " error" << endl;
	#endif
	return false;
    }
    return true;

}

template <typename T, unsigned int size>
bool NamedPipe<T, size>::writeData(T buf, unsigned int bufLen) {
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
    pid_t fd;
    if (openW(fd) == false) 
	return false;
    int len = write(fd, buf, bufLen);
    if (len == -1) { 
	#ifdef DEBUG
	cerr << "write error" << endl;
	cout << "write buf: " << buf << endl;
	cout << "read_buf size: " << size << endl;
	#endif
	close(fd);
	return false;
    }
    #ifdef DEBUG
    cout << "write buf: " << buf << endl;
    cout << "read_buf size: " << size << endl;
    cout << "write len: " << len << endl;
    #endif
    close(fd);
    return true;
}


template <typename T, unsigned int size>
bool NamedPipe<T, size>::readData(T buf, unsigned int bufLen) {
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
    pid_t fd;
    if (openR(fd) == false)
	return false;
    int len = read(fd, buf, size);
    if (len == -1) {
	#ifdef DEBUG
	cerr << "read error" << endl;
	cout << "read buf: " << buf << endl;
	cout << "read_buf size: " << size << endl;
	#endif
	close(fd);
	return false;
    }
    #ifdef DEBUG
    cout << "read buf: " << buf << endl;
    cout << "read_buf size: " << size << endl;
    cout << "read len: " << len << endl;
    #endif
    close(fd);
    return true;
}


} //end of namesapce zpt_ser

#endif
