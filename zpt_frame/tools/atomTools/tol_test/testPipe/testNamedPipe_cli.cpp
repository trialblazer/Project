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
    char* readBuf = new char[BUFSIZE];
    if (readBuf == NULL)
	return 0;
    string name(FIFONAME);
    cout << "in client" << endl;
    NamedPipe<char*, BUFSIZE> pipe(name, NoConsanguinity);
    if (pipe.readData(readBuf, BUFSIZE) == false)
	cout << "read error" << endl;
    delete readBuf;
    return 0;
}
