
#include <muduo/base/Condition.h>

#include <errno.h>

// returns true if time out, false otherwise.
bool muduo::Condition::waitForSeconds(double seconds)
{
//pthread_cond_timedwait(pthread_cond_t * cond, pthread_mutex_t *mutex, const struct timespec * abstime)
/*
多线程编程中，线程A循环计算，然后sleep一会接着计算（目的是减少CPU利用率）；存在的问题是，如果要关闭程序，
通常选择join线程A等待线程A退出，可是我们必须等到sleep函数返回，该线程A才能正常退出，这无疑减慢了程序退出的速度。
当然，你可以terminate线程A，但这样做很不优雅，且会存在一些未知问题。
*/
    struct timespec abstime;
//int clock_gettime(clockid_t clk_id,struct timespec *tp);
/*
    struct timespec
    {
    time_t tv_sec; //秒
    long tv_nsec; // 纳秒
    };
*/
    clock_gettime(CLOCK_REALTIME, &abstime); 
/*
    CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,
    中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
　　CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
　　CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间
　　CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间
*/

    const int64_t kNanoSecondsPerSecond = 1e9;  // 10^9  1s
    int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

    abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
    abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

    MutexLock::UnassignGuard ug(mutex_);
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(), &abstime);
}