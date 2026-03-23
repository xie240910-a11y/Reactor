#include "ThreadPool.h"
#include "Task.h"

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
:threadNum_(threadNum)
,threads_()
,queSize_(queSize)
,taskQue_(queSize_)
,isExit(false)
{}

ThreadPool::~ThreadPool()
{
    if(!isExit)
    {
        stop();
    }
}

// 线程池开始或者结束
void ThreadPool::start()
{
    //创建threadNum个工作线程，并存储到vector中
    for(size_t i = 0; i < threadNum_; ++i)
    {
        // threads_.emplace_back([this](){
        //     this->doTask();
        // });
        std::thread th(&ThreadPool::doTask, this);
        threads_.push_back(std::move(th));
    }
}

void ThreadPool::stop()
{
    while(!taskQue_.empty())
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    isExit = true;
    taskQue_.wakeup();
    //回收所有的工作线程
    for(auto &th : threads_)
    {
        th.join();
    }
}

//存放任务或者取出任务
void ThreadPool::addTask(Task *pTask)
{
    if(pTask){
        taskQue_.push(pTask);
    }
}

Task * ThreadPool::getTask()
{
    Task *task = taskQue_.pop();
    return task;
}

void ThreadPool::doTask()
{
    while(!isExit)
    {
        Task* task = getTask();
        if (task) {
            task->startProcess();
        } else {
            std::cout << "nullptr == task" << std::endl;
        }
    }
}
