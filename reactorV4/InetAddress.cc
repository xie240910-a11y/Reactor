#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress(const std::string &ip, unsigned short port)
{
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr) <= 0) {
        perror("inet_pton");
    }
}

// InetAddress::InetAddress(const string &ip, unsigned short port)
// { 
//     memset(&addr_, 0, sizeof(addr_)); // 清除addr_上的数据
//     addr_.sin_family = AF_INET;
//     addr_.sin_port = htons(port);
//     addr_.sin_addr.s_addr = inet_addr(ip.c_str());
// }

InetAddress::InetAddress(const struct sockaddr_in &addr)
:addr_(addr)
{}

const string InetAddress::getIp() const
{
    return string(inet_ntoa(addr_.sin_addr));
}
const unsigned short InetAddress::getPort() const
{
    return ntohs(addr_.sin_port);
}

const struct sockaddr_in *InetAddress::getAddr() const
{
    return &addr_;
}

socklen_t InetAddress::getAddrLen() const {
    return sizeof(addr_);
}