#include "epollpoller.h"
#include "channel.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <iostream>

namespace mihooke {

EpollPoller::EpollPoller() : _epollFd(epoll_create1(EPOLL_CLOEXEC)), _waitEvents(16)
{

}

EpollPoller::~EpollPoller()
{
    ::close(_epollFd);
}

int EpollPoller::poll(std::vector<Channel*> &activeChannels)
{
    int numEvents = ::epoll_wait(_epollFd, _waitEvents.data(), static_cast<int>(_waitEvents.size()), 5000);
    if (numEvents < 0)
    {
        abort();
    }
//    std::cout << "numEvents=" << numEvents << std::endl;
    for (int i=0; i<numEvents; ++i)
    {
        Channel *channel = static_cast<Channel*>(_waitEvents[i].data.ptr);
        channel->setRevents(_waitEvents[i].events);
        activeChannels.push_back(channel);
    }
    if (numEvents == _waitEvents.size())
    {
        _waitEvents.resize(numEvents * 2);
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

void EpollPoller::updateChannel(Channel *channel)
{
    using State = Channel::State;
    State state = channel->state();
    if (state == State::New || state == State::Deleted)
    {
        int fd = channel->fd();
        if (state == State::New)
        {
            _channels[fd] = channel;
        }
        channel->setState(State::Added);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setState(State::Deleted);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
        
    }
    
}

void EpollPoller::update(int operation, Channel *channel)
{
    struct epoll_event ev;
    ev.events = channel->events();
    ev.data.fd = channel->fd();
    ev.data.ptr = channel;
    int er = ::epoll_ctl(_epollFd, operation, channel->fd(), &ev);
    if (er < 0)
    {
        abort();
    }
}

} // namespace mihooke
