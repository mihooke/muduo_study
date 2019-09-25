#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "socket.h"
#include "epollpoller.h"

using namespace std;
using namespace mihooke;
int main()
{
    //// mihooke 注释: 创建监听fd
    int listenFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (listenFd < 0)
    {
        cout << "create socket error" << endl;
        return 1;
    }
    Socket listenSock(listenFd);
    listenSock.setReuseAddr(true);
    listenSock.setReusePort(true);
    listenSock.bind(9527);
    listenSock.listen();

    //// mihooke 注释: 将监听fd加入epoll中
    EpollPoller poller;
    poller.addEvent(EPOLLIN, listenFd);

    //// mihooke 注释: 开始等待IO事件
    while (true)
    {
        int numEvents = poller.poll();
//            cout<<"1"<<endl;
        const std::vector<struct epoll_event> waitEvents = poller.waitEvents();
        cout<<numEvents << " " << waitEvents[0].data.fd<<endl;

        for (int i=0; i<numEvents; ++i)
        {
            uint32_t revent = waitEvents[i].events;
            int rfd = waitEvents[i].data.fd;
            //// mihooke 注释: 监听fd有可读事件，则有新连接
            if ((revent & EPOLLIN) && (rfd == listenFd))
            {
                int connectFd = listenSock.accept();
                //// mihooke 注释: 设置新连接为非阻塞的
                int flags = fcntl(connectFd, F_GETFL, 0);
                flags |= O_NONBLOCK;
                fcntl(connectFd, F_SETFL, flags);
                poller.addEvent(EPOLLIN, connectFd);
            }
            //// mihooke 注释: 其他可读事件
            else if (revent & EPOLLIN)
            {
                char buf[1024];
                memset(buf, 0, 1024);
                long nRead = read(rfd, buf, 1024);
                if (nRead < 0)
                {
                    cout << "read error" << endl;
                    poller.delEvent(EPOLLIN, rfd);
                    close(rfd);
                }
                else if (nRead == 0)
                {
                    cout << "client closed" << endl;
                    poller.delEvent(EPOLLIN, rfd);
                    close(rfd);
                }
                else
                {
                    cout << "read message:" << buf << endl;
                    poller.modEvent(EPOLLIN, rfd);
                }
            }
            //// mihooke 注释: 可写事件
            else if (revent & EPOLLOUT)
            {
                char buf[] = "a message from server";
                long nWrite = write(rfd, buf, 1024);
                poller.modEvent(EPOLLIN, rfd);
            }
        }
    }

    return 0;
}
