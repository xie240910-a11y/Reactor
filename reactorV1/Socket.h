#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>

class Socket
{
public:
    Socket();
    explicit Socket(int fd);
    ~Socket();
    int getFd() const;
    void shutdownWrite();
private:
    int fd_; // 服务端socket
};
#endif