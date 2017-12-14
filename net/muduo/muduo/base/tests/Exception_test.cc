#include <muduo/base/Exception.h>
#include <stdio.h>

class Bar
{
 public:
  void test()
  {
    throw muduo::Exception("oops");
  }
};

void foo()
{
  Bar b;
  b.test();
}

int main()
{
  try
  {
    foo();
  }
  catch (const muduo::Exception& ex)
  {
    printf("reason:\n%s\n", ex.what());
    printf("stack trace:\n%s\n", ex.stackTrace());
  }
}

/*
reason: oops
stack trace: ./exception_test(_ZN5muduo9Exception14fillStackTraceEv+0x23) [0x4018e3]
./exception_test(_ZN5muduo9ExceptionC1EPKc+0x77) [0x401af7]
./exception_test() [0x4014b0]
./exception_test(main+0x6) [0x401326]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7fd76ab7ff45]
./exception_test() [0x4013c0]
*/
// demangle 执行后的输出
/*
reason:
oops
stack trace:
muduo::Exception::fillStackTrace()
muduo::Exception::Exception(char const*)
./exception_test()
./exception_test(main+0x6)
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5)
./exception_test()
*/