#include "Accetpor.h"

Acceptor::Acceptor(const string  &ip, unsigned short port)
: sock_()
, addr_(ip, port)
{}

void Acceptor::ready()
{
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}
int Acceptor::accept()
{
    int connFd = ::accept(sock_.getFd(), nullptr, nullptr);
    if (connFd == -1) {
        perror("accept");
        return -1;
    }
    return connFd;
}

int Acceptor::fd()
{
    return sock_.getFd();
}

void Acceptor::setReuseAddr()
{
    int on = 1;
    int ret = setsockopt(sock_.getFd(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret) {
        perror("setsockopt");
        return;
    }
}
void Acceptor::setReusePort()
{
    int on = 1;
    int ret = setsockopt(sock_.getFd(), SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    if (ret == -1) {
        perror("setsockopt");
        return;
    }
}
void Acceptor::bind()
{
    int ret = ::bind(sock_.getFd(), (struct sockaddr *)addr_.getAddr(), addr_.getAddrLen());
    if (ret == -1) {
        perror("bind");
        return;
    }
}
void Acceptor::listen()
{
    int ret = ::listen(sock_.getFd(), SOMAXCONN);
    if (ret == -1) {
        perror("listen");
        return;
    }
}