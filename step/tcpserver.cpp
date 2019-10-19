#include "tcpserver.h"
#include "eventloop.h"
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
    : _loop(loop), _port(port), _fd(createSocketFd()), _listenSock(_fd)
{
    //// mihooke 注释: 将监听fd加入epoll中
    _loop->addEventFd(_fd);

    _loop->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1));
    _loop->setListenFd(_fd);
}

void TcpServer::start()
{
    _listenSock.setReuseAddr(true);
    _listenSock.setReusePort(true);
    _listenSock.bind(_port);
    _listenSock.listen();
}

void TcpServer::newConnection(int fd)
{
    int connectFd = _listenSock.accept();
    //// mihooke 注释: 设置新连接为非阻塞的
    int flags = fcntl(connectFd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(connectFd, F_SETFL, flags);
    _loop->addEventFd(connectFd);
}
} // namespace mihooke
