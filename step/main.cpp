#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
int main()
{
    //// mihooke 注释: 创建监听fd
    int listenFd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
    if (listenFd < 0)
    {
        cout << "create socket error" << endl;
        return 1;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(9527);
    if (bind(listenFd, (sockaddr*)&serverAddr, sizeof serverAddr) < 0)
    {
        cout << "bind error" << endl;
        return 2;
    }
    if (listen(listenFd, SOMAXCONN) < 0)
    {
        cout << "listen error" << endl;
        return 3;
    }

    //// mihooke 注释: 将监听fd加入epoll中
    int epollFd = epoll_create1(EPOLL_CLOEXEC);
    struct epoll_event listenEvent, waitEvents[128];
    listenEvent.events = EPOLLIN;
    listenEvent.data.fd = listenFd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, listenFd, &listenEvent) < 0)
    {
        cout << "epoll_ctrl error" << endl;
        return 4;
    }

    //// mihooke 注释: 开始等待IO事件
    while (true)
    {
        int numEvents = epoll_wait(epollFd, waitEvents, 128, 5000);
        if (numEvents < 0)
        {
            cout << "epoll_wait error" << endl;
            return 5;
        }

        for (int i=0; i<numEvents; ++i)
        {
            uint32_t revent = waitEvents[i].events;
            int rfd = waitEvents[i].data.fd;
            struct sockaddr_in connectAddr;
            socklen_t connectAddrLen;
            //// mihooke 注释: 监听fd有可读事件，则有新连接
            if ((revent & EPOLLIN) && (rfd == listenFd))
            {
                int connectFd = accept(listenFd, (sockaddr*)&connectAddr, &connectAddrLen);
                if (connectFd < 0)
                {
                    cout << "accept error" << endl;
                    return 6;
                }
                cout << "a new connection:" << inet_ntoa(connectAddr.sin_addr) << endl;
                //// mihooke 注释: 设置新连接为非阻塞的
                int flags = fcntl(connectFd, F_GETFL, 0);
                flags |= O_NONBLOCK;
                fcntl(connectFd, F_SETFL, flags);
                listenEvent.data.fd = connectFd;
                listenEvent.events = EPOLLIN;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD, connectFd, &listenEvent) < 0)
                {
                    cout << "epoll_ctrl error" << endl;
                    return 7;
                }
            }
            //// mihooke 注释: 其他可读事件
            else if (revent & EPOLLIN)
            {
                char buf[1024];
                memset(buf, 0, 1024);
                int nRead = read(rfd, buf, 1024);
                if (nRead < 0)
                {
                    cout << "read error" << endl;
                    close(rfd);
                    listenEvent.data.fd = rfd;
                    listenEvent.events = EPOLLIN;
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, rfd, &listenEvent);
                }
                else if (nRead == 0)
                {
                    cout << "client closed" << endl;
                    close(rfd);
                    listenEvent.data.fd = rfd;
                    listenEvent.events = EPOLLIN;
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, rfd, &listenEvent);
                }
                else
                {
                    cout << "read message:" << buf << endl;
                    listenEvent.data.fd = rfd;
                    listenEvent.events = EPOLLOUT;
                    epoll_ctl(epollFd, EPOLL_CTL_MOD, rfd, &listenEvent);
                }
            }
            //// mihooke 注释: 可写事件
            else if (revent & EPOLLOUT)
            {
                char buf[] = "a message from server";
                int nWrite = write(rfd, buf, 1024);
                listenEvent.data.fd = rfd;
                listenEvent.events = EPOLLIN;
                epoll_ctl(epollFd, EPOLL_CTL_MOD, rfd, &listenEvent);
            }
        }
    }

    close(epollFd);
    return 0;
}
