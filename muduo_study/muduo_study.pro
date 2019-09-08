TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread

SOURCES += main.cpp \
    muduo/base/AsyncLogging.cc \
    muduo/base/Condition.cc \
    muduo/base/CountDownLatch.cc \
    muduo/base/CurrentThread.cc \
    muduo/base/Date.cc \
    muduo/base/Exception.cc \
    muduo/base/FileUtil.cc \
    muduo/base/LogFile.cc \
    muduo/base/Logging.cc \
    muduo/base/LogStream.cc \
    muduo/base/ProcessInfo.cc \
    muduo/base/Thread.cc \
    muduo/base/ThreadPool.cc \
    muduo/base/Timestamp.cc \
    muduo/base/TimeZone.cc \
    muduo/net/poller/DefaultPoller.cc \
    muduo/net/poller/EPollPoller.cc \
    muduo/net/poller/PollPoller.cc \
    muduo/net/Acceptor.cc \
    muduo/net/Buffer.cc \
    muduo/net/Channel.cc \
    muduo/net/Connector.cc \
    muduo/net/EventLoop.cc \
    muduo/net/EventLoopThread.cc \
    muduo/net/EventLoopThreadPool.cc \
    muduo/net/InetAddress.cc \
    muduo/net/Poller.cc \
    muduo/net/Socket.cc \
    muduo/net/SocketsOps.cc \
    muduo/net/TcpClient.cc \
    muduo/net/TcpConnection.cc \
    muduo/net/TcpServer.cc \
    muduo/net/Timer.cc \
    muduo/net/TimerQueue.cc

HEADERS += \
    muduo/base/AsyncLogging.h \
    muduo/base/Atomic.h \
    muduo/base/BlockingQueue.h \
    muduo/base/BoundedBlockingQueue.h \
    muduo/base/Condition.h \
    muduo/base/copyable.h \
    muduo/base/CountDownLatch.h \
    muduo/base/CurrentThread.h \
    muduo/base/Date.h \
    muduo/base/Exception.h \
    muduo/base/FileUtil.h \
    muduo/base/GzipFile.h \
    muduo/base/LogFile.h \
    muduo/base/Logging.h \
    muduo/base/LogStream.h \
    muduo/base/Mutex.h \
    muduo/base/noncopyable.h \
    muduo/base/ProcessInfo.h \
    muduo/base/Singleton.h \
    muduo/base/StringPiece.h \
    muduo/base/Thread.h \
    muduo/base/ThreadLocal.h \
    muduo/base/ThreadLocalSingleton.h \
    muduo/base/ThreadPool.h \
    muduo/base/Timestamp.h \
    muduo/base/TimeZone.h \
    muduo/base/Types.h \
    muduo/base/WeakCallback.h \
    muduo/net/poller/EPollPoller.h \
    muduo/net/poller/PollPoller.h \
    muduo/net/Acceptor.h \
    muduo/net/Buffer.h \
    muduo/net/Callbacks.h \
    muduo/net/Channel.h \
    muduo/net/Connector.h \
    muduo/net/Endian.h \
    muduo/net/EventLoop.h \
    muduo/net/EventLoopThread.h \
    muduo/net/EventLoopThreadPool.h \
    muduo/net/InetAddress.h \
    muduo/net/Poller.h \
    muduo/net/Socket.h \
    muduo/net/SocketsOps.h \
    muduo/net/TcpClient.h \
    muduo/net/TcpConnection.h \
    muduo/net/TcpServer.h \
    muduo/net/Timer.h \
    muduo/net/TimerId.h \
    muduo/net/TimerQueue.h \
    muduo/net/ZlibStream.h
