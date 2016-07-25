
#include "../../shm/shm.h"
#include "../../shm/shmCommon.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;
using namespace zpt_frame;

int main() {

    int pid;
    char writeBuf[] = "hello world";
    char readBuf[1024];
    memset(readBuf, '\0', 1024);
    if ((pid = fork()) == -1) {
	cerr <<  "fork" << endl;
	exit(1);
    }
    if (pid == 0) {

	sleep(2);
	cout << "in child" << endl;
	Shm<char*, 1024, Follower> s('c');
	s.readData(readBuf, 1024);
    } else if (pid > 0) {
	cout << "in parent" << endl;
	Shm<char*, 1024, Leader> s('c');
	s.writeData(writeBuf, strlen(writeBuf));
	sleep(6);
    }
    return 0;
}
