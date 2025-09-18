#include "Accetpor.h"
#include "EventLoop.h"
#include "TcpServer.h"

using std::cout;
using std::endl;

void onNewConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!!!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    string msg = con->receive();
    cout << con->clientPtr() << ">>recv msg from client: " << msg << endl;
    con->send("服务端已收到：" + msg);
}
void onClose(const TcpConnectionPtr &con)
{
    cout << con->clientPtr() << ">>has closed!!!" << endl;
}

void test0()
{
    Acceptor acceptor("127.0.0.1", 8888);
    acceptor.ready(); // 让服务器处于监听状态
    
    EventLoop loop(acceptor);
    loop.setNewConnectionCallback(onNewConnection);
    loop.setMessageCallback(onMessage);
    loop.setClose(onClose);

    loop.loop();

}

void test1()
{
    TcpServer server("127.0.0.1", 8888);
    server.setAllCallback(onNewConnection, onMessage, onClose);
    server.start();
}

int main(int argc, const char *argv[])
{
    test1();
    return 0;
}

