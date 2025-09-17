#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Accetpor.h"
#include "TcpConnection.h"
#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;

using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &)>;

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

};

#endif