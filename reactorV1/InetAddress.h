#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <arpa/inet.h> // inet_pton
#include <iostream>

using std::string;
class InetAddress
{
public:
    InetAddress(const string  &ip, unsigned short port);
    InetAddress(const struct sockaddr_in &addr);
    ~InetAddress() = default;
    const string getIp() const;
    const unsigned short getPort() const;
    const struct sockaddr_in *getAddr() const;
    socklen_t getAddrLen() const;
private:
    struct sockaddr_in addr_;
};
#endif