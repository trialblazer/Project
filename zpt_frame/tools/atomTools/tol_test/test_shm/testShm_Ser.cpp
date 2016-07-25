#include "../../shm/shm.h"
#include "../../shm/shmCommon.h"
#include <iostream>
#include <cstdlib>
#include <string.h>

using namespace std;
using namespace zpt_frame;

int main() {
    char writeBuf[1024];
    Shm<char*, 1024, Leader> s('c');
    while (1) {
	cin.getline(writeBuf, 1024);
	s.writeData(writeBuf, strlen(writeBuf));
    }
    return 0;
}
