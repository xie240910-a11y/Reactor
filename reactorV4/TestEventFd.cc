#include "EventFd.h"

#include <iostream>
#include <functional>
#include <thread>
#include <unistd.h>

using std::cout;
using std::endl;
using std::thread;
using std::bind;

class MyTask
{
public:
    void process() {
        cout <<  ">>MyTask is running" << endl;
    }    
};

void test() {
    MyTask task;
    EventFd fd(std::bind(&MyTask::process, &task));
    // fd.start();
    thread th(bind(&EventFd::start, &fd));
    int cnt = 10;
    while (cnt--) {
        fd.wakeup();
        sleep(1);
    }
    
    fd.stop();
    th.join();
}

int main(int argc, char const *argv[])
{
    test();
    return 0;
}
