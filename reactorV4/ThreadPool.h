#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.h"
#include <thread>
#include <vector>

class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();

    // 线程池开始或者结束
    void start();
    void stop();

    //存放任务或者取出任务
    void addTask(Task *pTask);

    void doTask();
private:
    Task *getTask();

    size_t threadNum_;
    std::vector<std::thread> threads_;
    size_t queSize_;
    TaskQueue taskQue_;
    bool isExit;
};


#endif
