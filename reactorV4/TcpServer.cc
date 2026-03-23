#include "TcpServer.h"

TcpServer::TcpServer(const string & ip, unsigned short port)
: acceptor_(ip, port)
, loop_(acceptor_)
{
    
}

TcpServer::~TcpServer()
{
    stop();
}

void TcpServer::start()
{
    // 让服务器处于监听状态
    acceptor_.ready();
    // 启动循环
    loop_.loop();
}
void TcpServer::stop()
{
    loop_.unloop();
}
void TcpServer::setAllCallback(TcpConnectionCallback connectCallback, TcpConnectionCallback messageCallback, TcpConnectionCallback closeCallback)
{
    loop_.setNewConnectionCallback(std::move(connectCallback));
    loop_.setMessageCallback(std::move(messageCallback));
    loop_.setClose(std::move(closeCallback));
}