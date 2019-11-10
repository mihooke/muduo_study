#include "tcpserver.h"
#include "eventloop.h"
#include "acceptor.h"
#include "channel.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <fcntl.h>
using namespace std;
using std::placeholders::_1;

namespace mihooke {

int createSocketFd()
{
    int listenFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (listenFd < 0)
    {
        cout << "create socket error" << endl;
        abort();
    }
    return listenFd;
}

TcpServer::TcpServer(EventLoop *loop, unsigned short port)
    : _loop(loop), _fd(createSocketFd()), _port(port), _acceptor(new Acceptor(loop, _fd, port))
{
    _acceptor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1));
}

TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    _acceptor->listen();
}

void TcpServer::newConnection(int fd)
{
    cout << "A new client connected:" << fd << endl;
    shared_ptr<Channel> channel(new Channel(_loop, fd));
    _connections[fd] = channel;
    channel->setReadEventCallback(bind(&TcpServer::handleRead, this));
    channel->enableReading();
}

void TcpServer::handleRead()
{
    cout << "A message from client" << endl;
    //// TODO 需要把fd中的数据读完,否则会一直触发可读事件，这就需要增加TcpConnection类
}

} // namespace mihooke
