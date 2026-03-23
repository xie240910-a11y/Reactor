#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include "Accetpor.h"
#include "EventLoop.h"

class TcpServer
{
public:
    TcpServer(const string & ip, unsigned short port);
    ~TcpServer();
    void start();
    void stop();
    void setAllCallback(TcpConnectionCallback connectCallback, TcpConnectionCallback messageCallback, TcpConnectionCallback closeCallback);
private:
    Acceptor acceptor_;
    EventLoop loop_;
};

#endif