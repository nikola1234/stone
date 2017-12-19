#ifndef MUDUO_BASE_CURRENTTHREAD_H
#define MUDUO_BASE_CURRENTTHREAD_H

#include <stdint.h>
namespace muduo
{

namespace CurrentThread
{
  // internal
  extern __thread int t_cachedTid;
  extern __thread char t_tidString[32];
  extern __thread int t_tidStringLength;
  extern __thread const char* t_threadName;

  void cacheTid();

  inline int tid()
  {
    //if (unlikely(t_cachedTid == 0))  //绝大多数 t_cachedTid！= 0 即 t_cachedTid == 0 为false
    if (__builtin_expect(t_cachedTid == 0, 0))
    {
      cacheTid();
    }
    return t_cachedTid;   //预先读取这个
  }

  inline const char* tidString() // for logging
  {
    return t_tidString;
  }

  inline int tidStringLength() // for logging
  {
    return t_tidStringLength;
  }

  inline const char* name()
  {
    return t_threadName;
  }

  bool isMainThread();

  void sleepUsec(int64_t usec);
}   // CurrentThread end

}   //  muduo end

#endif  //MUDUO_BASE_CURRENTTHREAD_H