#ifndef CHANNEL_H
#define CHANNEL_H
#include <memory>
#include <functional>


namespace mihooke {
class EventLoop;

class Channel
{
public:
    enum State
    {
        New = -1,
        Added = 1,
        Deleted = 2
    };
    typedef std::function<void()> EventCallback;
    typedef std::function<void()> ReadEventCallback;

    Channel(EventLoop *loop, int fd);
    void handleEvent();
    void enableReading() { _events |= kReadEvent; update(); }
    void disableReading() { _events &= ~kReadEvent; update(); }
    void enableWriting() { _events |= kWriteEvent; update(); }
    void disableWriting() { _events &= ~kWriteEvent; update(); }
    void disableAll() { _events = kNoneEvent; update(); }
    bool isNoneEvent() const { return _events == kNoneEvent; }

    void setReadEventCallback(ReadEventCallback cb) { _readEventCallback = std::move(cb); }
    void setWriteEventCallback(EventCallback cb) { _writeEventCallback = std::move(cb); }
    void setCloseEventCallback(EventCallback cb) { _closeEventCallback = std::move(cb); }
    void setErrorEventCallback(EventCallback cb) { _errorEventCallback = std::move(cb); }

    int fd() const { return _fd; }
    int events() const { return _events; }
    void setRevents(int revents) { _revents = revents; }

    State state() const { return _state; }
    void setState(State state) { _state = state; }

private:
    void update();

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *_loop;
    int _fd;
    int _events{kNoneEvent};
    int _revents{kNoneEvent};
    State _state{New};

    ReadEventCallback _readEventCallback;
    EventCallback _writeEventCallback;
    EventCallback _closeEventCallback;
    EventCallback _errorEventCallback;
};
} // namespace mihooke

#endif // CHANNEL_H
