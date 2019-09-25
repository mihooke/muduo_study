#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
namespace mihooke {

class Socket
{
public:
    explicit Socket(int fd) : _fd(fd)
    {}
    ~Socket();

    void bind(const uint16_t port);
    void listen();
    int accept();

    int fd() const {return _fd;}

    void shutdownWrite();

    void setReuseAddr(bool on);
    void setReusePort(bool on);
private:
    const int _fd;
};
} // namespace mihooke

#endif // SOCKET_H
