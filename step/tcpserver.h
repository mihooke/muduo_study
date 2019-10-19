#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <functional>
#include <memory>
#include "socket.h"


namespace mihooke {
class EventLoop;

class TcpServer
{
public:
    typedef std::function<void (int sockfd)> NewConnectionCallback;
    TcpServer(EventLoop *loop, unsigned short port);
    void start();

    void newConnection(int fd);

private:
    EventLoop *_loop;
    int _fd;
    Socket _listenSock;
    unsigned short _port;
};
} // namespace mihooke

#endif // TCPSERVER_H
