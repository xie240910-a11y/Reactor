#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "SocketIO.h"
#include "Socket.h"
#include "InetAddress.h"
#include "NonCopyable.h"

using std::string;
class TcpConnection
:public NonCopyable
{
public:
    TcpConnection(int fd);
    ~TcpConnection() = default;
    string receive();
    void send(const string & msg);

    //方便调试函数
    string toString();
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    SocketIO sockIO_;

    //为调试加入三个数据成员
    Socket sock_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
};
#endif