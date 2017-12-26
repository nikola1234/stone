#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Types.h>

#include <deque>
#include <vector>

namespace muduo
{
class ThreadPool : noncopyable
{
public:
    typedef std::function<void ()> Task;   // 任务

    explicit ThreadPool(const string& nameArg = string("ThreadPool"));
    ~ThreadPool();

    void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; } // 任务队列最大值
    void setThreadInitCallback(const Task& cb)
    { threadInitCallback_ = cb; }

    void start(int numThreads);  // 设定线程数量
    void stop();

    const string& name() const
    { return name_; }

    size_t queueSize() const;

    // Could block if maxQueueSize > 0
    void run(Task f);
  
private:
    bool isFull() const;
    void runInThread();
    Task take();

    mutable MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    string name_;
    Task threadInitCallback_;
    std::vector<std::unique_ptr<muduo::Thread>> threads_;   // 线程向量
    std::deque<Task> queue_;                // 任务队列
    size_t maxQueueSize_;
    bool running_;  //线程池运行标志位
};
}

#endif
