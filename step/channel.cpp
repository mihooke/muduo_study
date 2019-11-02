#include "channel.h"
#include "eventloop.h"

#include <poll.h>
#include <sys/epoll.h>
#include <iostream>
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace mihooke
{

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *loop, int fd) : _loop(loop), _fd(fd)
{
}

void Channel::handleEvent()
{
    if ((_revents & EPOLLHUP) && !(_revents & EPOLLIN))
    {
        if (_closeEventCallback)
            _closeEventCallback();
    }

    if (_revents & POLLNVAL)
    {
        std::cout << "fd = " << _fd << " Channel::handleEvent() EPOLLNVAL" << std::endl;
    }

    if (_revents & (EPOLLERR | POLLNVAL))
    {
        if (_errorEventCallback)
            _errorEventCallback();
    }
    if (_revents & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
//        char buf[1024];
//        memset(buf, 0, 1024);
//        long nRead = read(_fd, buf, 1024);
//        if (nRead < 0)
//        {
//            std::cout << "read error" << std::endl;
//            close(_fd);
//        }
//        else if (nRead == 0)
//        {
//            std::cout << "client closed" << std::endl;
//            close(_fd);
//            disableAll();
//        }
//        else
//        {
//            std::cout << "read message:" << buf << std::endl;
//            enableReading();
//        }
        if (_readEventCallback)
            _readEventCallback();
    }
    if (_revents & EPOLLOUT)
    {
        if (_writeEventCallback)
            _writeEventCallback();
    }
}

void Channel::update()
{
    _loop->updateChannel(this);
}

} // namespace mihooke
