#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <memory>
#include <map>


namespace mihooke {
class Channel;
class EventLoop;
class Acceptor;

class TcpServer
{
public:
    TcpServer(EventLoop *loop, unsigned short port);
    ~TcpServer();
    void start();

    void newConnection(int fd);
    void handleRead();

private:
    EventLoop *_loop;
    int _fd;
    unsigned short _port;
    std::unique_ptr<Acceptor> _acceptor;
    std::map<int, std::shared_ptr<Channel>> _connections;
};
} // namespace mihooke

#endif // TCPSERVER_H
