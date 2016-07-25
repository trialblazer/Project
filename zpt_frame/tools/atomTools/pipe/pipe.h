#ifndef PIPE_H_
#define PIPE_H_

#include <string>
#include <iostream>
#include "pipeCommon.h"
#include "namedPipe.h"
#include "noNamePipe.h"

namespace zpt_ser {

using std::string;


//依次为通信数据类型，通信单元大小，管道类型，通信类型
template <class T, unsigned int size, PipeType PT, CommunicateType CT>
class PIPE {//:public BasePipe<T> {
private:
//规定_rPipe为server端的读端，为client的写端。_wPipe为server的写端，为client的读端
    BasePipe<T>* _rPipe;//读管道描述符
    BasePipe<T>* _wPipe;//写管道描述符

    string _wrName; //写管道名称
    string _rdName; //读管道名称

    PipeCurOwn own; //管道当前使用者身份

public:
    PIPE(string, string);
    virtual ~PIPE();
    void initOwn(PipeCurOwn o) { own = o; }
    bool readData(T buf, unsigned int bufLen); //从管道中读取数据
    bool writeData(T buf, unsigned int bufLen);  //往管道中写入数据
}; //end of class PIPE


template <class T, unsigned int size, PipeType PT, CommunicateType CT>
PIPE<T, size, PT, CT>::PIPE(string wn, string rn) {
    own = NoSet;
    _wrName = wn;
    _rdName = rn;
    if (PT == Named) { //创建有名管道
	_wPipe = new NamedPipe<T, size>(_wrName, CT);
	if (_wPipe == NULL) {
	    cerr << "the pipe of write point create error" << endl;
	    exit(1);
	}
	_rPipe = new NamedPipe<T, size>(_rdName, CT);//创建管道
	if (_rPipe == NULL) {// //创建管道失败
	    cerr << "the pipe of read point create error" << endl;
	    exit(1);
	}
    } else { //创建无名管道
	_wPipe = new NoNamePipe<T, size>();
	if (_wPipe == NULL) {
	    cerr << "the pipe of write point create error" << endl;
	    exit(1);
	}
	_rPipe = new NoNamePipe<T, size>();
	if (_rPipe == NULL) {
	    cerr << "the pipe of read point create error" << endl;
	    exit(1);
	}
    }
}


template <class T, unsigned int size, PipeType PT, CommunicateType CT>
PIPE<T, size, PT, CT>::~PIPE() {
    if (_rPipe != NULL)
	delete _rPipe;
    if (_wPipe != NULL) 
	delete _wPipe;
    _rPipe = NULL;
    _wPipe = NULL;
}



template <class T, unsigned int size, PipeType PT, CommunicateType CT>
bool PIPE<T, size, PT, CT>::readData(T buf, unsigned int bufLen) {
    using namespace std;
    if (own == NoSet) {
	cout << "please set own" << endl;
	return false;
    }
    if (own == Client)
	return _wPipe->readData(buf, bufLen);  
    else 
	return _rPipe->readData(buf, bufLen); 
}


template <class T, unsigned int size, PipeType PT, CommunicateType CT>
bool PIPE<T, size, PT, CT>::writeData(T buf, unsigned int bufLen) {
    using namespace std;
    if (own == NoSet) {
	cout << "please set own" << endl;
	return false;
    }
    if (own == Server)
	return _wPipe->writeData(buf, bufLen);  
    else 
	return _rPipe->writeData(buf, bufLen); 
}



} // end of namespace zpt_ser

#endif // end of PIPE_H
