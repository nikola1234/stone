#include <muduo/base/Thread.h>

#include <unistd.h>
#include <sys/syscall.h> // ::syscall
#include <sys/types.h>

namespace muduo
{

namespace CurrentThread
{
  /*
    1、__thread 关键字只能修饰POD(Plain Old Data)类型的数据- C风格的struct结构体定义的数据结构
      std::cout << std::boolalpha << std::is_pod<T>::value 检查是否为POD类型
    2、__thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。__thread变量每一个线程有一份独立实体
    各个线程的值互不干扰。可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。
    3、线程中非POD类型数据也可以用“线程特定数据，也被称为线程私有数据”存储。
  */
  __thread int t_cachedTid = 0; //缓存 thread id
  __thread char t_tidString[32];
  __thread int t_tidStringLength = 6;
  __thread const char* t_threadName = "unknown";
  //检查pid_t的类型是否是 int  有些机器上的pid_t是long
  static_assert(std::is_same<int, pid_t>::value, "pid_t should be int");
}   // CurrentThread end

namespace detail
{
pid_t gettid()
{
  /*
    每个进程有一个pid，类型pid_t，由getpid()取得
    线程id : pthread_self()  //进程内唯一，但是在不同进程则不唯一。
    syscall(SYS_gettid)   //系统内是唯一的
  */
  return static_cast<pid_t>(::syscall(SYS_gettid));
}
void afterFork()
{
  muduo::CurrentThread::t_cachedTid = 0;
  muduo::CurrentThread::t_threadName = "main";
  CurrentThread::tid();
  // no need to call pthread_atfork(NULL, NULL, &afterFork);
}

class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    muduo::CurrentThread::t_threadName = "main";
    CurrentThread::tid();
    pthread_atfork(NULL, NULL, &afterFork);
  }
};

ThreadNameInitializer init; // 初始化主线程线程信息。

}
}   //  muduo end


using namespace muduo;

void CurrentThread::cacheTid()
{
  if (t_cachedTid == 0) // 线程变量获取 Tid
  {
    t_cachedTid = detail::gettid();
    t_tidStringLength = snprintf(t_tidString, sizeof(t_tidString), "%5d ", t_cachedTid);
  }
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func,  const string& name)
  : started_(false),
    joined_(false),
    pthreadId_(0),
    tid_(0),
    func_(std::move(func)),
    name_(name)
{
  setDefaultName();
}

Thread::~Thread()
{

}

void Thread::setDefaultName()
{
  int num = numCreated_.incrementAndGet();
  if (name_.empty())
  {
    char buf[32];
    snprintf(buf, sizeof(buf), "Thread%d", num);
    name_ = buf;
  }
}

void Thread::start()
{
  pthread_create()
}