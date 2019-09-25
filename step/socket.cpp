#include "socket.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
using std::cout;
using std::endl;

namespace mihooke {

Socket::~Socket()
{
    close(_fd);
}

void Socket::bind(const uint16_t port)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof serverAddr);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(port);
    if (::bind(_fd, (sockaddr*)&serverAddr, sizeof serverAddr) < 0)
    {
        cout << "bind error" << endl;
        abort();
    }
}

void Socket::listen()
{
    if (::listen(_fd, SOMAXCONN) < 0)
    {
        cout << "listen error" << endl;
        abort();
    }}

int Socket::accept()
{
    struct sockaddr_in connectAddr;
    memset(&connectAddr, 0, sizeof connectAddr);
    socklen_t connectAddrLen = 0;
    int connectFd = ::accept(_fd, (sockaddr*)&connectAddr, &connectAddrLen);
    if (connectFd < 0)
    {
        cout << "accept error: " << connectFd << endl;
        abort();
    }
    return connectFd;
}

void Socket::shutdownWrite()
{

}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT,
                 &optval, static_cast<socklen_t>(sizeof optval));
}

} // namespace mihooke
