#ifndef EPOLLPOLLER_H
#define EPOLLPOLLER_H

#include <vector>

struct epoll_event;
namespace mihooke {

class EpollPoller
{
public:
    EpollPoller();

    ~EpollPoller();

    int poll();
    std::vector<struct epoll_event> waitEvents() const {return _waitEvents;}

    void addEvent(unsigned int event, int fd);
    void modEvent(unsigned int event, int fd);
    void delEvent(unsigned int event, int fd);

private:
    void update();

    int _epollFd;
    std::vector<struct epoll_event> _waitEvents;
};
} // namespace mihooke

#endif // EPOLLPOLLER_H
