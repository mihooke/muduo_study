#include "epollpoller.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>

namespace mihooke {

EpollPoller::EpollPoller() : _epollFd(epoll_create1(EPOLL_CLOEXEC)), _waitEvents(16)
{

}

EpollPoller::~EpollPoller()
{
    ::close(_epollFd);
}

int EpollPoller::poll()
{
    int numEvents = ::epoll_wait(_epollFd, _waitEvents.data(), static_cast<int>(_waitEvents.size()), 5000);
    if (numEvents < 0)
    {
        abort();
    }
    return numEvents;
}

void EpollPoller::addEvent(unsigned int event, int fd)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (::epoll_ctl(_epollFd, EPOLL_CTL_ADD, fd, &ev) < 0)
    {
        abort();
    }
}

void EpollPoller::modEvent(unsigned int event, int fd)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (::epoll_ctl(_epollFd, EPOLL_CTL_MOD, fd, &ev) < 0)
    {
        abort();
    }
}

void EpollPoller::delEvent(unsigned int event, int fd)
{
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (::epoll_ctl(_epollFd, EPOLL_CTL_DEL, fd, &ev) < 0)
    {
        abort();
    }
}

void EpollPoller::update()
{

}

} // namespace mihooke
