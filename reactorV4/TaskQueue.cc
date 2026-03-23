#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t capa)
:capacity_(capa)
,que_()
,mutex_()
,notFull_()
,notEmpty()
,flag(true)

{

}

TaskQueue::~TaskQueue()
{

}

//生产数据与消费数据
void TaskQueue::push(ElemType ptask)
{
    unique_lock<mutex> ul(mutex_);
    //判满
    while(full())
    {
        // 如果任务队列是满的，生产者等待
        notFull_.wait(ul);
    }

    //如果任务队列不满， 将数据插入队列唤醒消费者
    que_.push(ptask);
    notEmpty.notify_one();
}

TaskQueue::ElemType TaskQueue::pop()
{
    unique_lock<mutex> ul(mutex_);
    //判空
    while(empty() && flag)
    {
        // 如果任务队列是满的，生产者等待
        notEmpty.wait(ul);
    }
    if (flag)
    {
        ElemType tmp =  que_.front();
        que_.pop();
        notFull_.notify_one();
        return tmp;
    } else {
        return nullptr;
    }
}

bool TaskQueue::empty() const
{
    return que_.size() == 0;
}
bool TaskQueue::full() const
{
    return que_.size() == capacity_;
}

void TaskQueue::wakeup()
{
    flag = false;
    notEmpty.notify_all();
}