#include <iostream>
#include "thread.h"

using namespace std;
using namespace zpt_frame;

void* f(void* t=NULL) {
    while (1) {
	cout << "a  " << flush;
	sleep(2);
    }
    return NULL;
}

void* g(void* t=NULL) {
    while (1) {
	cout << "b " << flush;
	sleep(2);
    }
}

int main() {
    Thread t1(f);
    if (t1._initState == Failure) {
	cout << "error" << endl;
    }
    
    Thread t2(g); 
    if (t2._initState == Failure) {
	cout << "error" << endl;
	return 0;
    }
    pthread_t a[2];
    a[0] = t1.getThreadId();
    a[1] = t2.getThreadId();
    Thread::wait(a, 2);


    return 0;
}
