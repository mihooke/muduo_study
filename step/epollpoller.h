#ifndef EPOLLPOLLER_H
#define EPOLLPOLLER_H

#include <vector>
#include <map>

struct epoll_event;
namespace mihooke {
class Channel;

class EpollPoller
{
public:
    EpollPoller();

    ~EpollPoller();

    int poll(std::vector<Channel*> &activeChannels);
    std::vector<struct epoll_event> waitEvents() const {return _waitEvents;}

    void updateChannel(Channel *channel);
private:
    void update(int operation, Channel *channel);

    int _epollFd;
    std::vector<struct epoll_event> _waitEvents;
    std::map<int, Channel*> _channels;
};
} // namespace mihooke

#endif // EPOLLPOLLER_H
