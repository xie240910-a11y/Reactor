#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Accetpor.h"
#include "TcpConnection.h"
#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <functional>
#include <mutex>

using std::vector;
using std::map;
using std::shared_ptr;
using std::mutex;
using std::lock_guard;
using Functor = std::function<void()>;
class EventLoop
{
public:
    
    EventLoop(const Acceptor &acceptor);
    ~EventLoop();
    // 循环或者关闭循环
    void loop();
    void unloop();
    // 封装epoll_wait
    void wailEpollFd();
    
    // 处理新的连接请求
    void handleNewConnection();
    // 处理老的连接上的数据收发
    void handleMessage(int fd);
    // 封装epoll_create
    int createEpollFd();
    //封装epoll_ctl，将文件描述符放到红黑树上监听
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);

    // eventFd相关方法；
    void wakeup();
    int createEventFd();
    void handleRead();
    void doPendingFunctors();

    void runInloop(Functor &&cb);
public:
    // 三个事件注册
    void setNewConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback && cb);
    void setClose(TcpConnectionCallback && cb);

private:
    int epfd_;
    vector<struct epoll_event> evtList_;
    bool isLooping_;
    Acceptor acceptor_;
    map<int, shared_ptr<TcpConnection>> conns_;

    TcpConnectionCallback onNewConnection_; // 连接建立
    TcpConnectionCallback onMessage_; // 消息到达
    TcpConnectionCallback onClose_; // 连接断开

    // 线程通信
    int evtfd_;
    vector<Functor> pendings_;
    mutex mutex_;
};

#endif