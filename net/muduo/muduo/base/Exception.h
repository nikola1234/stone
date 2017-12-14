#ifndef MUDUO_BASE_EXCEPTION_H
#define MUDUO_BASE_EXCEPTION_H

#include <muduo/base/Types.h>  // string类和转换方法
#include <exception>

namespace muduo
{

// std::exception
//        +virtual const char* what() const 返回解释性字符串

class Exception : public std::exception
{
 public:
  explicit Exception(const char* what);
  explicit Exception(const string& what);
  virtual ~Exception() throw();              // throw () //不抛出任何异常

  // 返回抛出异常的内容
  virtual const char* what() const throw();  // throw () //不抛出任何异常
  // 打印栈的信息
  const char* stackTrace() const throw();    // throw () //不抛出任何异常

 private:
  void fillStackTrace();
  string demangle(const char* symbol);

  string message_;  //  throw muduo::Exception("oops"); 抛出异常的内容 oops
  string stack_;    // 栈信息 
};

}

#endif