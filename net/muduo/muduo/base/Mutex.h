#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include <muduo/base/CurrentThread.h>
#include <muduo/base/noncopyable.h>
#include <assert.h>
#include <pthread.h>

namespace muduo
{

class MutexLock : noncopyable
{

};

//RAII,资源获取就是初始化，
class MutexLockGuard : noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
    :mutex_(mutex)
    {
        mutex_.lock();
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }
private:
    MutexLock& mutex_;
};

}

#define MutexLockGuard(x) error "Missing guard object name"

#endif  // MUDUO_BASE_MUTEX_H
