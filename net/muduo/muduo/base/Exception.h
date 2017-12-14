#ifndef MUDUO_BASE_EXCEPTION_H
#define MUDUO_BASE_EXCEPTION_H

#include <muduo/base/Types.h>  // string类和转换方法
#include <exception>

namespace muduo
{

class Exception : public std::exception
{
 public:
  explicit Exception(const char* what);
  explicit Exception(const string& what);
  virtual ~Exception() throw();              // throw () //不抛出任何异常

  virtual const char* what() const throw();  // throw () //不抛出任何异常
  const char* stackTrace() const throw();    // throw () //不抛出任何异常

 private:
  void fillStackTrace();
  string demangle(const char* symbol);

  string message_;
  string stack_;
};

}

#endif