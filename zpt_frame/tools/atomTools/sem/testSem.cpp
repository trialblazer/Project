#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include "sem.h"

using namespace std;
using namespace zpt_frame;


int main() {
    int key = 1;
    Sem<Leader> s(2);
    s.setVal(0);
    while (1) {
	if (s.semP() == false)
	    cout << "wait" << endl;
	sleep(2);
    }
/*    pid_t pid = fork();
    if (pid > 0) {
	Sem<Leader> s(2);
	s.setVal(1);
	while (1) {
	    s.semP(); 
	    cout << endl;
	    cout << "in parent val: " << s.getVal() << endl;
	    key++;
	    cout << "in parent key++  " << key << endl;
	    sleep(2);
	    s.semV();
	}
	int status;
	waitpid(pid, &status, NULL); //等待子进程结束
    } else if (pid == 0 ) {
	Sem<Follower> t(2);
	while (1) {
	    t.semP();
	    cout << endl;
	    cout << "int child val: " << t.getVal() << endl;
	    key++;
	    cout << "in child key++ " << key << endl;
	    t.semV();
	}
    } else 
	cout << "error" << endl;
	*/
    return 0;
}
