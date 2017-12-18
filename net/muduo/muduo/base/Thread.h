
#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <muduo/base/Atomic.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Types.h>

#include <functional>
#include <pthread.h>

namespace muduo
{

class Thread : noncopyable
{
 public:
    typedef std::function<void ()> ThreadFunc;
    explicit Thread(ThreadFunc,  const string& name = string());
    ~Thread();

    void start();

 private:
    void setDefaultName();

    ThreadFunc func_;
    string     name_;
    pthread_t  pthreadId_;
    pid_t      tid_;
    bool       started_;
    bool       joined_;

    static AtomicInt32 numCreated_;
};

}

#endif
