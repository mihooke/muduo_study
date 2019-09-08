#include <iostream>
#include <functional>
#include <memory>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using std::placeholders::_1;

class EchoServer
{
public:
    EchoServer(EventLoop *loop, const InetAddress& listenAddr)
        : _loop(loop), _server(loop, listenAddr, "Echoserver")
    {
        _server.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
    }
    void start() {_server.start();}
private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        LOG_INFO << "A client comes:" << conn->localAddress().toIpPort();
    }

    EventLoop *_loop;
    TcpServer _server;
};

int main()
{
    EventLoop loop;
    EchoServer server(&loop, InetAddress(12345));
    server.start();
    loop.loop();
    return 0;
}
