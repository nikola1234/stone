/*
首先是一个队列
其次是没有大小限制的

通过条件变量通知队列是否空或有数据
*/


#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <deque>
#include <assert.h>

namespace muduo
{

template<typename T>
class BlockingQueue : noncopyable
{
public: 
    BlockingQueue()
      : mutex_(),
        notEmpty_(mutex_),
        queue_()  //默认构造函数
    {

    }

    void put (const T& x)
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(x);
        notEmpty_.notify();
    }

    void put (T&& x)  // 右值不能是const关键字修饰
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(std::move(x)); // 右值要用 std::move
        notEmpty_.notify();
    }

    T take()
    {
        MutexLockGuard lock(mutex_);  //锁是对队列操作的锁
        while (queue_.empty())
        {
            notEmpty_.wait();
        }
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return std::move(front);
    }

    size_t size() const
    {
        MutexLockGuard lock(mutex_);
        return queue_.size();
    }
private:
    mutable MutexLock mutex_;  // 条件变量的锁  mutable是在const的函数中能够改变 mutex
    Condition      notEmpty_;  // 
    std::deque<T>  queue_;

};

}

#endif  // MUDUO_BASE_BLOCKINGQUEUE_H

