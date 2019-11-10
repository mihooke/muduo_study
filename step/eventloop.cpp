#include "eventloop.h"
#include "epollpoller.h"
#include "channel.h"

#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
namespace mihooke {

EventLoop::EventLoop() : _poller(new EpollPoller)
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::loop()
{
    while (true)
    {
        //// mihooke 注释: need clear before new loop
        _activeChannels.clear();
        int numEvents = _poller->poll(_activeChannels);

        for (Channel *channel : _activeChannels)
        {
            channel->handleEvent();
        }
    }
}

void EventLoop::setNewConnectionCallback(const NewConnectionCallback &cb)
{
    _newConnectionCb = cb;
}

void EventLoop::updateChannel(Channel *channel)
{
    _poller->updateChannel(channel);
}
} // namespace mihooke
