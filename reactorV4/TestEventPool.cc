#include "Accetpor.h"
#include "EventLoop.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "Task.h"
using std::cout;
using std::endl;
using std::string;

ThreadPool pool(4, 10);

class MyTask
:public Task
{
public:
    explicit MyTask(const string &msg, const TcpConnectionPtr &con)
    : msg_(msg)
    , con_(con)
    {}

    ~MyTask() = default;
    void startProcess() {
        msg_;
        con_->sendInLoop(msg_);
    }
private:
    string msg_;
    TcpConnectionPtr con_;
};
void onNewConnection(const TcpConnectionPtr &con)
{
    cout << con->toString() << " has connected!!!" << endl;
}

void onMessage(const TcpConnectionPtr &con)
{
    string msg = con->receive();
    cout << con->clientPtr() << ">>recv msg from client: " << msg << endl;
    // con->send("服务端已收到：" + msg);
    MyTask task(msg, con);
    pool.addTask((Task *) &std::bind(&MyTask::startProcess, task));
    con->send(msg);
}
void onClose(const TcpConnectionPtr &con)
{
    cout << con->clientPtr() << ">>has closed!!!" << endl;
}

void test0()
{
    
    pool.start();
    TcpServer  server("127.0.0.1", 8888);
    server.setAllCallback(onNewConnection, onMessage, onClose);
    server.start();
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

