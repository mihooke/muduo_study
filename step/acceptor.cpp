#include "acceptor.h"
#include <fcntl.h>
namespace mihooke {

Acceptor::Acceptor(EventLoop *loop, int fd, unsigned short port): _loop(loop),_listenFd(fd), 
        _listenSock(fd), _acceptChannel(loop, fd)
{
    _listenSock.setReuseAddr(true);
    _listenSock.setReusePort(true);
    _listenSock.bind(port);
    _acceptChannel.setReadEventCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen()
{
    _listenSock.listen();
    _acceptChannel.enableReading();
}

void Acceptor::handleRead()
{
    int connectFd = _listenSock.accept();
    //// mihooke 注释: 设置新连接为非阻塞的
    int flags = fcntl(connectFd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(connectFd, F_SETFL, flags);

    if (connectFd >= 0)
    {
        if (_newConnectionCb)
            _newConnectionCb(connectFd);
    }
}


} // namespace mihooke
