
#include "../../shm/shm.h"
#include "../../shm/shmCommon.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;
using namespace zpt_frame;

int main() {
    char readBuf[512];
    Shm<char*, 1024, Follower> s('c');
    while (1)  {
	memset(readBuf, '\0', 1024);
	s.readData(readBuf, 512);
	if (strlen(readBuf) == 0)
	    sleep(2);
    }
    return 0;
}
