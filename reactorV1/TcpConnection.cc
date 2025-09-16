#include "TcpConnection.h"
#include <iostream>
#include <sstream>
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