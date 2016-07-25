#include "../namedPipe.h"
#include "../common.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#define BUFSIZE 1024

int main() {
    using namespace zpt_ser;
    char writeBuf[20] = "hello";
    string name(FIFONAME);
    NamedPipe<char*, BUFSIZE> pipe(name, NoConsanguinity);

    pid_t pid;
    cout << "in server" << endl;
    if (pipe.writeData(writeBuf, strlen(writeBuf)) == false)
	cout << "write error" << endl;
    return 0;
}
