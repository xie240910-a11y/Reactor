#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "EventLoop.h"
#include "SocketIO.h"
#include "Socket.h"
#include "InetAddress.h"
#include "NonCopyable.h"
#include <memory>
#include <functional>

using std::string;
using std::function;
using std::shared_ptr;

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

class TcpConnection
: public std::enable_shared_from_this<TcpConnection>
{
    
public:
    explicit TcpConnection(int fd, EventLoop *loop);
    ~TcpConnection() = default;
    string receive();
    void send(const string & msg);

    //方便调试函数
    string toString();
    string clientPtr();

    InetAddress getLocalAddr();
    InetAddress getPeerAddr();

public:
    // 三个事件注册
    void setNewConnectionCallback(const TcpConnectionCallback & cb);
    void setMessageCallback(const TcpConnectionCallback & cb);
    void setClose(const TcpConnectionCallback & cb);

    // 事件执行
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handlesetClose();

    bool isClosed();

    void sendInLoop(const string &msg);
private:
    SocketIO sockIO_;

    //为调试加入三个数据成员
    Socket sock_;
    InetAddress localAddr_;
    InetAddress peerAddr_;

    TcpConnectionCallback onNewConnection_; // 连接建立
    TcpConnectionCallback onMessage_; // 消息到达
    TcpConnectionCallback onClose_; // 连接断开
    EventLoop *loop_;
};
#endif