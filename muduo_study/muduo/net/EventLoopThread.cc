// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/EventLoopThread.h"

#include "muduo/net/EventLoop.h"

using namespace muduo;
using namespace muduo::net;

//// mihooke 注释
//// 线程池线程构造：构造线程对象
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
                                 const string& name)
  : loop_(NULL),
    exiting_(false),
    thread_(std::bind(&EventLoopThread::threadFunc, this), name),
    mutex_(),
    cond_(mutex_),
    callback_(cb)
{
}

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  if (loop_ != NULL) // not 100% race-free, eg. threadFunc could be running callback_.
  {
    // still a tiny chance to call destructed object, if threadFunc exits just now.
    // but when EventLoopThread destructs, usually programming is exiting anyway.
    loop_->quit();
    thread_.join();
  }
}

//// mihooke 注释
//// 启动线程，也就是执行EventLoopThread::threadFunc，这里会等待直到生成一个loop
//// 注意：mutex和condition一起使用，mutex不会等待释放
//// 对这两个函数的写法有疑问：startLoop()里为什么不直接生成一个loop对象？然后在threadFunc()里直接开启循环？
//// 一个可能的原因是：目前muduo的做法loop是stack object，如果按照疑问里做法，loop需要是heap Object
//// 同样的，如果EventLoop loop_，由于EventLoopThread是new出来的，所以loop_也是heap object
//// 释疑：新线程里需要运行loop()，线程池里需保存loop指针，但线程池不知道何时loop对象产生，就需要等待；个人想法：loop对象也可由线程池生成管理
EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();

  EventLoop* loop = NULL;
  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL)
    {
      cond_.wait();
    }
    loop = loop_;
  }

  return loop;
}

//// mihooke 注释
//// 此函数就是一个无限循环，从这里可以看出，每个线程都会启动一个loop，时刻监听事件
void EventLoopThread::threadFunc()
{
  EventLoop loop;

  if (callback_)
  {
    callback_(&loop);
  }

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
  //assert(exiting_);
  MutexLockGuard lock(mutex_);
  loop_ = NULL;
}

