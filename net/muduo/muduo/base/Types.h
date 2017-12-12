#ifndef MUDUO_BASE_TYPES_H
#define MUDUO_BASE_TYPES_H

#include <stdint.h>              // int8_t unit8_t等类型

#ifdef MUDUO_STD_STRING          // 根目录中CMakeList.txt中打开这个宏定义就可以使用c++中的string类
#include <string>
#else  // !MUDUO_STD_STRING
#include <ext/vstring.h>         // 使用gnu的__sso_string 类
#include <ext/vstring_fwd.h>
#endif

#ifndef NDEBUG                   // -DNDEBUG 取消检查， 默认debug模式
#include <assert.h>
#endif

#include <muduo/base/noncopyable.h>

namespace muduo
{

#ifdef MUDUO_STD_STRING
using std::string;
#else  // !MUDUO_STD_STRING
typedef __gnu_cxx::__sso_string string;  // 兼容 c++ string 和 gnu string
#endif

template<typename To, typename From>
inline To implicit_cast(From const &f)   // 将隐式转换显示写出来 
{
  return f;
}

template<typename To, typename From>
inline To down_cast(From* f)            // 向下转型  
{
  if (false)     // 条件保证了最终肯定会被编译器优化掉，所以对性能没有任何影响。
  {
    implicit_cast<From*, To>(0);
  }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
  // dynamic_cast 只用于对象的指针和引用,是否是父类指针向派生类指针转换， 失败返回 NULL
  // 本身就是NULL，或者转换后不为NULL 才是正确的转换。
  assert(f == NULL || dynamic_cast<To>(f) != NULL);  // RTTI: debug mode only!  运行时debug模式启动assert
#endif
  // static_cast 可以在父类和子类中做转换，向上或向下
  return static_cast<To>(f);
}

}

#endif