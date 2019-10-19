#include "eventloop.h"
#include "epollpoller.h"
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
namespace mihooke {

EventLoop::EventLoop() : _poller(new EpollPoller)
{

}

void EventLoop::loop()
{
    while (true)
    {
        int numEvents = _poller->poll();
        const std::vector<struct epoll_event> waitEvents = _poller->waitEvents();
        std::cout<<numEvents << " " << waitEvents[0].data.fd<<" " <<_listenFd<<std::endl;

        for (int i=0; i<numEvents; ++i)
        {
            uint32_t revent = waitEvents[i].events;
            int rfd = waitEvents[i].data.fd;
            //// mihooke 注释: 监听fd有可读事件，则有新连接
            if ((revent & EPOLLIN) && (rfd == _listenFd))
            {
                _newConnectionCb(rfd);
            }
            //// mihooke 注释: 其他可读事件
            else if (revent & EPOLLIN)
            {
                char buf[1024];
                memset(buf, 0, 1024);
                long nRead = read(rfd, buf, 1024);
                if (nRead < 0)
                {
                    std::cout << "read error" << std::endl;
                    _poller->delEvent(EPOLLIN, rfd);
                    close(rfd);
                }
                else if (nRead == 0)
                {
                    std::cout << "client closed" << std::endl;
                    _poller->delEvent(EPOLLIN, rfd);
                    close(rfd);
                }
                else
                {
                    std::cout << "read message:" << buf << std::endl;
                    _poller->modEvent(EPOLLIN, rfd);
                }
            }
            //// mihooke 注释: 可写事件
            else if (revent & EPOLLOUT)
            {
                char buf[] = "a message from server";
                long nWrite = write(rfd, buf, 1024);
                _poller->modEvent(EPOLLIN, rfd);
            }
        }
    }
}

void EventLoop::addEventFd(int fd)
{
    _poller->addEvent(EPOLLIN, fd);
}

void EventLoop::setNewConnectionCallback(const NewConnectionCallback &cb)
{
    _newConnectionCb = cb;
}
} // namespace mihooke
