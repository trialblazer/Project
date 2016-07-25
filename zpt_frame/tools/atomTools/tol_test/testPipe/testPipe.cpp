#include "../pipe.h" 
#include "../pipeCommon.h"
#include <iostream>
#include <string.h>
#include <string>

const unsigned int BUFSIZE = 1024;

using namespace std;
using namespace zpt_ser;

string name1 =  "fifo1";
string name2 = "fifo2";

char buf[BUFSIZE];
char writeBuf1[] = "hello world";
char writeBuf2[] = "you are good";

//test noNamePipe
void testNoNameConsanguinity() {
    PIPE<char*, BUFSIZE, NoName, Consanguinity> pipe(name1, name2); 

    pid_t pid;
    if ((pid=fork()) == -1) {
	cerr << "fork" << endl;
	exit(1);
    }
    if (pid == 0) {
	cout << "in child" << endl;
	pipe.initOwn(Client);
	if (pipe.writeData(writeBuf1, strlen(writeBuf1)) == false)
	    cout << "write error" << endl;
	sleep(2);
	if (pipe.readData(buf, BUFSIZE) == false) {
	    cout << "read error" << endl;
	}
    } else if (pid > 0) {
	cout << "in parent" << endl;
	pipe.initOwn(Server);
	if (pipe.writeData(writeBuf2, strlen(writeBuf2)) == false)
	    cout << "read error" << endl;
	sleep(2);
	if (pipe.readData(buf, BUFSIZE) == false)
	    cout << "read error" << endl;
	    
    }
    
}



void testNamedConsanguinity() {
    PIPE<char*, BUFSIZE, Named, NoConsanguinity> pipe(name1, name2); 
    pid_t pid;
    if ((pid=fork()) == -1) {
	cerr << "fork" << endl;
	exit(1);
    }
    if (pid == 0) {
	cout << "in child" << endl;
	pipe.initOwn(Client);
	if (pipe.writeData(writeBuf1, strlen(writeBuf1)) == false)
	    cout << "write error" << endl;
	sleep(2);
	if (pipe.readData(buf, BUFSIZE) == false) {
	    cout << "read error" << endl;
	}
    } else if (pid > 0) {
	cout << "in parent" << endl;
	pipe.initOwn(Server);
	if (pipe.writeData(writeBuf2, strlen(writeBuf2)) == false)
	    cout << "read error" << endl;
	sleep(2);
	if (pipe.readData(buf, BUFSIZE) == false)
	    cout << "read error" << endl;
	    
    }
}

void testNamedNoConsanguinity() {
    PIPE<char*, BUFSIZE, Named, NoConsanguinity> pipe(string(name1), string(name2)); 
}

int main() {
    //cout << "test no name consanguinity" << endl;
    //testNoNameConsanguinity();
    cout << endl << endl << "test no name noconsanguinity" <<endl;
    testNamedConsanguinity();
    testNamedNoConsanguinity();
    return 0;
}
