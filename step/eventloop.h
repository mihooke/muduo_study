#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <memory>
#include <functional>
#include <sys/epoll.h>


namespace mihooke {

class EpollPoller;

class EventLoop
{
public:
    typedef std::function<void (int sockfd)> NewConnectionCallback;
    EventLoop();
    void loop();

    void addEventFd(int fd);
    void setNewConnectionCallback(const NewConnectionCallback &cb);
    void setListenFd(int fd) {_listenFd = fd;}

private:
    std::unique_ptr<EpollPoller> _poller;
    NewConnectionCallback _newConnectionCb;
    int _listenFd{};
};
} // namespace mihooke

#endif // EVENTLOOP_H
