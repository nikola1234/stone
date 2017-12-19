#include <muduo/base/Thread.h>
#include <muduo/base/CurrentThread.h>

#include <string>
#include <stdio.h>
#include <unistd.h>

void threadFunc()
{
  printf("tid=%d\n", muduo::CurrentThread::tid());
}

int main()
{
  printf("pid=%d, tid=%d\n", ::getpid(), muduo::CurrentThread::tid());

  muduo::Thread t1(threadFunc);
  
}