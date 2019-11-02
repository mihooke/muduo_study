#ifndef TCPSERVER_H
#define TCPSERVER_H
//#include <functional>
#include <memory>
#include <map>


namespace mihooke {
class EventLoop;
class Acceptor;
class Channel;

class TcpServer
{
public:
//    typedef std::function<void (int sockfd)> NewConnectionCallback;
    TcpServer(EventLoop *loop, unsigned short port);
    void start();

    void newConnection(int fd);
    void handleRead();

private:
    EventLoop *_loop;
    int _fd;
    unsigned short _port;
//    std::unique_ptr<Acceptor> _acceptor;
    Acceptor *_acceptor;
    std::map<int, std::shared_ptr<Channel>> _connections;
};
} // namespace mihooke

#endif // TCPSERVER_H
