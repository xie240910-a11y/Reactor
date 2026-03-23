#ifndef __EVENTFD_H__
#define __EVENTFD_H__

#include <functional>
using EventFdCallback = std::function<void()>;


class EventFd
{
public:
    EventFd(EventFdCallback &&cb);
    EventFd(const EventFdCallback &cb);
    ~EventFd();
    
    // 启动停止
    void start();
    void stop();
    // 唤醒
    void wakeup();
    
private:
    int createEventFd();
    void handleRead();
private:
    int evtfd_;
    EventFdCallback cb_;
    bool isStarted_;
};

#endif