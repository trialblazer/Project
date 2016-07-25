#include "../noNamePipe.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#define BUFSIZE 1024

int main() {
    using namespace zpt_ser;
    char writeBuf[20] = "hello";
    char* readBuf = new char[BUFSIZE];
    if (readBuf == NULL)
	return 0;
    NoNamePipe<char*, BUFSIZE> pipe;

    pid_t pid;
    if ((pid=fork()) == -1) {
	cerr << "fork" << endl;
	exit(1);
    }
    if (pid == 0) {
	cout << "in child" << endl;
	if (pipe.writeData(writeBuf, strlen(writeBuf)) == false)
	    cout << "write error" << endl;
    } else if (pid > 0) {
	sleep(2);
	cout << "in parent" << endl;
	if (pipe.readData(readBuf, BUFSIZE) == false)
	    cout << "read error" << endl;
    }
    delete readBuf;
    return 0;
}
