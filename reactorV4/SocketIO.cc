#include "SocketIO.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

SocketIO::SocketIO(int fd)
:fd_(fd)
{}
int SocketIO::readn(char *buf, int len)
{
    int left = len;
    char *pstr = buf;
    int ret = 0;
    while(left > 0)
    {
        ret = read(fd_, pstr, left);
        if (ret == -1 && errno == EINTR) {
            continue;
        } else if (ret == -1) {
            perror("read");
            return -1;
        } else if (ret == 0) {
            break;
        } else {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}
int SocketIO::writen(const char *buf, int len)
{
    int left = len;
    const char *pstr = buf;
    int ret = 0;
    while(left > 0)
    {
        ret = write(fd_, pstr, left);
        if (ret == -1 && errno == EINTR) {
            continue;
        } else if (ret == -1) {
            perror("read");
            return -1;
        } else if (ret == 0) {
            break;
        } else {
            pstr += ret;
            left -= ret;
        }
    }
    return len - left;
}
int SocketIO::readLine(char *buf, int len)
{
    int left = len - 1;
    char *pstr = buf;
    int ret = 0, total = 0;
    while(left > 0)
    {
        ret = recv(fd_, pstr, left, MSG_PEEK);
        if (ret == -1 && errno == EINTR) {
            continue;
        } else if (ret == -1) {
            perror("readLine error -1");
            return -1;
        } else if (ret == 0) {
            break;
        } else {
            for (int idx = 0; idx < ret; ++idx) {
                if (pstr[idx] == '\n') {
                    int sz = idx + 1;
                    readn(pstr, sz);
                    pstr += sz;
                    *pstr = '\0';
                    return total + sz;
                }
            }

            readn(pstr, ret);
            total += ret;
            pstr += ret;
            left -= ret;

        }
    }
    *pstr = '\0';
    return total - left;
}
