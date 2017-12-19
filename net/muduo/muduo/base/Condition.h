
#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include <muduo/base/Mutex.h>

#include <pthread.h>

namespace muduo
{

class Condition : noncopyable
{
public:
    explicit Condition(MutexLock& mutex)
    : mutex_(mutex)
    {
        // 动态创建
        MCHECK(pthread_cond_init(&pcond_, NULL));
    }

    ~Condition()
    {
        MCHECK(pthread_cond_destroy(&pcond_));
    }
    //无条件等待pthread_cond_wait()和计时等待pthread_cond_timedwait()
    void wait()
    {
        MutexLock::UnassignGuard ug(mutex_);
        // pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，
        //然后阻塞在等待对列里休眠，直到再次被唤醒（大多数情况下是等待的条件成立而被唤醒，
        //唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，再读取资源  
        /*block-->unlock-->wait() return-->lock*/ 
        MCHECK(pthread_cond_wait(&pcond_, mutex_.getPthreadMutex())); //阻塞当前线程，等待signal和broadcast唤醒
    }

    // returns true if time out, false otherwise.
    bool waitForSeconds(double seconds);

    void notify()
    {
        MCHECK(pthread_cond_signal(&pcond_));
    }

    void notifyAll()
    {
        MCHECK(pthread_cond_broadcast(&pcond_));
    }

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};

}
#endif  // MUDUO_BASE_CONDITION_H
