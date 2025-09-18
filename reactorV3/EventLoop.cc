#include "EventLoop.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(const Acceptor &acceptor)
: epfd_(createEpollFd())
, evtList_(1024)
, isLooping_(false)
, acceptor_(acceptor)
,conns_()
{
    int listenfd = acceptor_.fd();
    addEpollReadFd(listenfd);
}

EventLoop::~EventLoop()
{
    close(epfd_);
}

int EventLoop::createEpollFd()
{
    int fd = epoll_create1(0);
    if (fd < 0) {
        perror("createEpollFd");
        return -1;
    }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt{};
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &evt);
    if (ret < 0) {
        perror("addEpollReadFd");
        return;
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt{};
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, &evt);
    if (ret < 0) {
        perror("delEpollReadFd");
        return;
    }
}

void EventLoop::loop()
{
    isLooping_ = true;
    while (isLooping_) {
        wailEpollFd();
    }
}

void EventLoop::unloop()
{
    isLooping_ = false;
}

void EventLoop::wailEpollFd()
{
    int nready = 0;
    int listenfd = acceptor_.fd();
    do {
        nready = epoll_wait(epfd_, evtList_.data(), evtList_.size(), 3000);
    } while(nready == -1 && errno == EINTR);

    if(nready == -1) {
        perror("epoll_wait");
        return;
    } else if (nready == 0) {
        printf(">> epoll_wail timeout!\n");
    } else {
        // 防止越界
        if (nready == (int)evtList_.size()) {
            evtList_.resize(2 * nready);
        }
        // 遍历struct epoll_event数组，去check
        // 每一个epoll_event到底发生了什么
        for(int idx = 0; idx < nready; ++idx) {
            int fd = evtList_[idx].data.fd;
            if (fd == listenfd) {
                if (evtList_[idx].events & EPOLLIN) {
                    //处理新连接
                    handleNewConnection();
                }
            } else {
                if (evtList_[idx].events & EPOLLIN) {
                    // 处理老的数据手法
                    handleMessage(fd); 
                }
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int connfd = acceptor_.accept();
    addEpollReadFd(connfd); //监听文件描述符
    TcpConnectionPtr con(new TcpConnection(connfd));
    con->setNewConnectionCallback(onNewConnection_);
    con->setMessageCallback(onMessage_);
    con->setClose(onClose_);
    conns_[connfd] = con;
    con->handleNewConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto it = conns_.find(fd);
    if (conns_.end() != it) {
        bool flag = it->second->isClosed(); //判断连接是否断开
        if (flag)
        {
            // 连接断开
            it->second->handlesetClose();
            // 需要将文件描述符从红黑树上删除
            delEpollReadFd(fd);
            conns_.erase(it);
        } else {
            it->second->handleMessageCallback();
        }
        // string msg = it->second->receive();
        // string clientStr = it->second->getPeerAddr().getIp();
        // cout << ">>recv msg from client(" << clientStr << ")" << msg << endl;
        // it->second->send("服务端已收到：" + msg);
    } else {
        cout << "该连接不存在" << endl;
        return;
    }
}


void EventLoop::setNewConnectionCallback(TcpConnectionCallback && cb)
{
    onNewConnection_ = std::move(cb);
}
void EventLoop::setMessageCallback(TcpConnectionCallback && cb)
{
    onMessage_ = std::move(cb);
}
void EventLoop::setClose(TcpConnectionCallback && cb)
{
    onClose_ = std::move(cb);
}