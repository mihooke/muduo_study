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
    socklen_t connectAddrLen;
    int connectFd = ::accept(_fd, (sockaddr*)&connectAddr, &connectAddrLen);
    if (connectFd < 0)
    {
        cout << "accept error" << endl;
        abort();
    }
    return connectFd;
}

void Socket::shutdownWrite()
{

}

} // namespace mihooke
