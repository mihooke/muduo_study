#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <memory>
#include <functional>

#include "channel.h"
#include "socket.h"

namespace mihooke {
class EventLoop;

class Acceptor
{
public:
    typedef std::function<void (int sockfd)> NewConnectionCallback;
    Acceptor(EventLoop *loop, int fd, unsigned short port);
    void listen();
    void setNewConnectionCallback(const NewConnectionCallback &cb) {_newConnectionCb = cb;}

private:
    void handleRead();

    EventLoop *_loop;
    int _listenFd;
    Socket _listenSock;
    Channel _acceptChannel;
    NewConnectionCallback _newConnectionCb;
};
} // namespace mihooke

#endif // ACCEPTOR_H
