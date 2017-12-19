#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include <muduo/base/CurrentThread.h>
#include <muduo/base/noncopyable.h>
#include <assert.h>
#include <pthread.h>

#ifdef CHECK_PTHREAD_RETURN_VALUE

#ifdef NDEBUG   //NDEBUG禁用assert
__BEGIN_DECLS   // c++代码中引用C标准库代码
extern void __assert_perror_fail (int errnum,
                                  const char *file,
                                  unsigned int line,
                                  const char *function) 
    __THROW __attribute__ ((__noreturn__));// Likewise, but prints the error text for ERRNUM.
__END_DECLS
#endif

//__typeof__(var) 是gcc对C语言的一个扩展保留字，用于声明变量类型,var可以是数据类型（int， char*..),也可以是变量表达式
#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       if (__builtin_expect(errnum != 0, 0))    \
                         __assert_perror_fail (errnum, __FILE__, __LINE__, __func__);})

#else  // CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})  //NDEBUG禁用assert

#endif // CHECK_PTHREAD_RETURN_VALUE

namespace muduo
{

class MutexLock : noncopyable
{
public:
    MutexLock()
    : holder_(0)
    {
        // 默认快速互斥锁 ,动态创建
        MCHECK(pthread_mutex_init(&mutex_, NULL)); 
    }
    ~MutexLock()
    {
        assert(holder_ == 0); // 判断是否线程释放了该锁
        MCHECK(pthread_mutex_destroy(&mutex_));
    }

    bool isLockedByThisThread() const // 判断是否是本线程定义了
    {
        return holder_ == CurrentThread::tid();
    }

    void assertLocked() const
    {
        assert(isLockedByThisThread());
    }

    pthread_mutex_t* getPthreadMutex() /* non-const */
    {
        return &mutex_;
    }

    void lock()
    {
        MCHECK(pthread_mutex_lock(&mutex_));
        assignHolder();
    }

    void unlock()
    {
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&mutex_));
    }

private:
    friend class Condition; //友缘类Condition
    class UnassignGuard : noncopyable // pthread_cond_wait用
    {
    public:
        UnassignGuard(MutexLock& owner)
          : owner_(owner)
        {
          owner_.unassignHolder();
        }

        ~UnassignGuard()
        {
          owner_.assignHolder();
        }

    private:
        MutexLock& owner_;
    };

    void unassignHolder()
    {
        holder_ = 0;
    }
    void assignHolder() // 获取线程tid
    {
        holder_ = CurrentThread::tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;  // 在哪一个线程中定义使用
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
    MutexLock& mutex_;   //利用引用，不对mutex资源的初始化和释放
};

}
//预防一下的书写定义临时MutexLockGuard变量
#define MutexLockGuard(x) error "Missing guard object name"

#endif  // MUDUO_BASE_MUTEX_H
