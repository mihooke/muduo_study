#include "epollpoller.h"
#include "tcpserver.h"
#include "eventloop.h"

using namespace std;
using namespace mihooke;
int main()
{
    EventLoop loop;
    TcpServer server(&loop, 9527);
    server.start();
    loop.loop();
    return 0;
}
