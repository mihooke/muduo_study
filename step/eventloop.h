#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <memory>
#include <functional>
#include <vector>
#include <sys/epoll.h>


namespace mihooke {

class EpollPoller;
class Channel;

class EventLoop
{
public:
    typedef std::function<void (int sockfd)> NewConnectionCallback;
    EventLoop();
    ~EventLoop();
    void loop();

    void setNewConnectionCallback(const NewConnectionCallback &cb);

    void updateChannel(Channel *channel);

private:
    std::unique_ptr<EpollPoller> _poller;
    NewConnectionCallback _newConnectionCb;
    std::vector<Channel*> _activeChannels;
};
} // namespace mihooke

#endif // EVENTLOOP_H
