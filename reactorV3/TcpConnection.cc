#include "TcpConnection.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::ostringstream;

TcpConnection::TcpConnection(int fd)
: sockIO_(fd)
, sock_(fd)
, localAddr_(getLocalAddr())
, peerAddr_(getPeerAddr())
{

}
string TcpConnection::receive()
{
    char buff[65535] = {0};
    sockIO_.readLine(buff, sizeof(buff));
    return string(buff);
}
void TcpConnection::send(const string & msg)
{
    sockIO_.writen(msg.c_str(), msg.size());
}

//方便调试函数
string TcpConnection::toString()
{
    ostringstream oss;
    oss << localAddr_.getIp() << ":"
        << localAddr_.getPort() << " ---->"
        << peerAddr_.getIp() << ":"
        << peerAddr_.getPort();
    return oss.str();
}

string TcpConnection::clientPtr()
{
    ostringstream oss;
    oss << peerAddr_.getIp() << ":"
        << peerAddr_.getPort();
    return oss.str();
}

InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    // 获取本端地址的函数getsockname，存放在第二个参数内
    int ret = getsockname(sock_.getFd(), (struct sockaddr *)&addr, &len);
    if (ret == -1)
    {
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr);
    // 获取对端地址的函数getpeername，存放在第二个参数内
    int ret = getpeername(sock_.getFd(), (struct sockaddr *)&addr, &len);
    if (ret == -1)
    {
        perror("getpeername");
    }
    return InetAddress(addr);
}

void TcpConnection::setNewConnectionCallback(const TcpConnectionCallback & cb)
{
    onNewConnection_ = cb;
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback & cb)
{
    onMessage_ = cb;
}
void TcpConnection::setClose(const TcpConnectionCallback & cb)
{
    onClose_ = cb;
}

void TcpConnection::handleNewConnectionCallback()
{
    if (onNewConnection_) {
        onNewConnection_(shared_from_this());
    } else {
        cout << "onNewConnection_ == nullptr" << endl;
    }
}
void TcpConnection::handleMessageCallback()
{
    if (onMessage_) {
        onMessage_(shared_from_this());
    } else {
        cout << "onMessage_ == nullptr" << endl;
    }
}
void TcpConnection::handlesetClose()
{
    if (onClose_) {
        onClose_(shared_from_this());
    } else {
        cout << "onClose_ == nullptr" << endl;
    }
}

bool TcpConnection::isClosed()
{
    char buf[10] = {0};
    int ret = recv(sock_.getFd(), buf, sizeof(buf), MSG_PEEK);
    return (ret == 0);
}