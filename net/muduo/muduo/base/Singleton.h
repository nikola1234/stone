#ifndef MUDUO_BASE_SINGLETON_H
#define MUDUO_BASE_SINGLETON_H

#include <muduo/base/noncopyable.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h> // atexit

namespace muduo
{

namespace detail
{
// This doesn't detect inherited member functions!
// http://stackoverflow.com/questions/1966362/sfinae-to-check-for-inherited-member-functions
template<typename T>
struct has_no_destroy  //SFINAE 技术，即匹配失败不是错误
{
    // decltype 编译器分析表达式并得到它的类型,c++11 用来替代 auto
    // decltype(&C::no_destroy)判断类中是否有no_destroy，并且分析这个函数的类型，返回void
    template<typename C> static char test(decltype(&C::no_destroy)); 
    template<typename C> static int32_t test(...); //没有就匹配这个函数
    const static bool value = sizeof(test<T>(0)) == 1;
};

}

template<typename T>
class Singleton : noncopyable
{
public: 
    static T& instance()
    {
        //int pthread_once(pthread_once_t *once_control,void(*init_routine)(void));
        pthread_once(&ponce_, &Singleton::init);  // 多线程中只执行一次，作为一个库，并不一定是在main函数中声明。
        assert(value_ != NULL);
        return *value_;
    }

private:
  Singleton();
  ~Singleton();

  static void init()
  {
    value_ = new T();

    if (!detail::has_no_destroy<T>::value)  // 判断
    {
       ::atexit(destroy);  //登记终止处理函数
    }
  }

  static void destroy()
  {
    // T 必须是完整类型，即不是class A {};
    // 如果是不完整类型，第一句就是  typedef char T_must_be_complete_typ[-1]，编译报错
    // 复杂类型定义 char[1]
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;

    delete value_;
    value_ = NULL;
  }

  static pthread_once_t   ponce_;  //定义为一个静态变量
  static T*               value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_=PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

}

#endif
