#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include <iostream>
#include "Socket.h"
#include "InetAddress.h"

class Acceptor
{
public:
    Acceptor(const string  &ip, unsigned short port);
    ~Acceptor() = default;
    void ready();
    int accept();
    int fd();
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
private:
    Socket sock_;
    InetAddress addr_;
};
#endif