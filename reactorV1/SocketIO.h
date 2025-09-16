#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__

class SocketIO
{
public:
    SocketIO(int fd);
    ~SocketIO() = default;
    int readn(char *buf, int len);
    int writen(const char *buf, int len);
    int readLine(char *buf, int len);

private:
    int fd_;
};
#endif