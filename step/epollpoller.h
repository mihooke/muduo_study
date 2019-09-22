#ifndef EPOLLPOLLER_H
#define EPOLLPOLLER_H

namespace mihooke {

class EpollPoller
{
public:
    EpollPoller();

    ~EpollPoller();

    void poll();

private:
    int _epollFd;

};
} // namespace mihooke

#endif // EPOLLPOLLER_H
