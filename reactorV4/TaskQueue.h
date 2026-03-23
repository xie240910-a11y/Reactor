#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

using std::queue;
using std::condition_variable;
using std::unique_lock;
using std::mutex;

class Task; //前向声明

class TaskQueue
{
    using ElemType = Task *;
public:
    TaskQueue(size_t capa);
    ~TaskQueue();
    
    // 生产数据与消费数据
    void push(ElemType ptask);
    ElemType pop();

    //判空和判满
    bool empty() const;
    bool full() const;

    //唤醒所有线程
    void wakeup();

private:
    size_t capacity_; //任务队列大小
    queue<ElemType> que_; //存放数据的数据结构
    mutex mutex_; //互斥锁
    condition_variable notFull_; //判满
    condition_variable notEmpty; //判空
    bool flag;
};

#endif