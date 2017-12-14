#include <muduo/base/Exception.h>

#include <cxxabi.h>
#include <execinfo.h> // backtrace（）获取当前线程的函数调用堆栈
#include <stdlib.h>
#include <stdio.h>

using namespace muduo;

Exception::Exception(const char* msg)
  : message_(msg)
{
  fillStackTrace();
}

Exception::Exception(const string& msg)
  : message_(msg)
{
  fillStackTrace();
}

Exception::~Exception() throw ()
{
}

const char* Exception::what() const throw()
{
  return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
  return stack_.c_str();
}

void Exception::fillStackTrace()
{
  const int len = 200;
  void* buffer[len];
  // int backtrace(void **buffer,int size)  
  // buffer是一个void*的数组
  // nptrs是返回能够获取的栈信息数组长度，最大200
  // man backtrace查询详细信息
  int nptrs = ::backtrace(buffer, len);
  // backtrace_symbols将从backtrace函数获取的信息转化为一个字符串数组
  // 需要 -rdynamic 这个编译选项支持
  char** strings = ::backtrace_symbols(buffer, nptrs);
  if (strings)
  {
    for (int i = 0; i < nptrs; ++i)
    {
      // TODO demangle funcion name with abi::__cxa_demangle
      stack_.append(demangle(strings[i]));
      stack_.push_back('\n');
    }
    free(strings);  // 特别注意需要自己释放资源
  }
}
// 函数名被编译器解释成另外一种风格
// 通过__cxa_demangle恢复
string Exception::demangle(const char* symbol)
{
  size_t size;
  int status;
  char temp[128];
  char* demangled;
  //first, try to demangle a c++ name
  if (1 == sscanf(symbol, "%*[^(]%*[^_]%127[^)+]", temp)) {
    if (NULL != (demangled = abi::__cxa_demangle(temp, NULL, &size, &status))) {
      string result(demangled);
      free(demangled);
      return result;
    }
  }
  //if that didn't work, try to get a regular c symbol
  if (1 == sscanf(symbol, "%127s", temp)) {
    return temp;
  }
 
  //if all else fails, just return the symbol
  return symbol;
}