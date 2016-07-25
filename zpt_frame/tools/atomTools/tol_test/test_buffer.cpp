#include "../buffer.h"
#include <iostream>
#include <string.h>

using namespace std;
using namespace zpt_frame;


class Msg {
public:
    int type;
    char data[0];

public:
    ~Msg() { delete data;}
    void set(int t, void* d, int size) {
	type = t;
	memcpy(data, d, size);
    }
    friend ostream& operator << (ostream& os, Msg& m) {
	os << m.type << "   " <<  m.data << endl;
	return os;
    }
    int getsize() { return sizeof(int)+sizeof(char)*strlen(data); }
};

int main() {
    Buffer buf;
    char b[20] = "heallo worl";
    char c[10] = "enghlo";
    Msg* msg = (Msg*)new char[sizeof(b)+sizeof(int)];
    Msg* _msg = (Msg*)new char[sizeof(c)+sizeof(int)];
    msg->set(1, b, sizeof(b));
    _msg->set(2, c, sizeof(c));
    cout << "msg size: " << msg->getsize() << endl;
    buf.writeBuf((void*)msg, msg->getsize());
    cout << "msg size: " << _msg->getsize() << endl;
    buf.writeBuf((void*)_msg, _msg->getsize());
    int size;
    while (!buf.empty()) {
	cout << "no empty" << endl;
	void* tmpt = buf.readBuf(size);
	cout << *((Msg*)(tmpt));
    }  
    cout << "empty" << endl;
    return 0;
}
