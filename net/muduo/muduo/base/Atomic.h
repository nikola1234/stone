#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H
// 无锁化编程，原子操作

#include <muduo/base/noncopyable.h>
#include <stdint.h>

namespace muduo
{

namespace detail
{
//通常提到并发编程，在数据的一致性上面，都会考虑用加锁方式在解决
//加锁其实并不耗时间，但是其容易造成资源等待
//所以用原子操作 __sync函数type可以是1,2,4或8字节长度的int类型
template<typename T>
class AtomicIntegerT : noncopyable  // 默认是私有继承  struct 默认是public
{
 public:
  AtomicIntegerT() : value_(0) {}

  T get()  // 返回value_值
  {
    // in gcc >= 4.7: __atomic_load_n(&value_, __ATOMIC_SEQ_CST)
    //地址上的值是否是期望的值，如果是，则更新该地址上的值为一个新的值
    /* *ptr == oldval newval写入*ptr,
    bool __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...) 相等并写入的情况下返回true.
    type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)  返回操作之前的值。
    */
    return __sync_val_compare_and_swap(&value_, 0, 0);  // CAS
  }

  T getAndAdd(T x)
  {
    // in gcc >= 4.7: __atomic_fetch_add(&value_, x, __ATOMIC_SEQ_CST)
    //type __sync_fetch_and_sub (type *ptr, type value, ...)
    return __sync_fetch_and_add(&value_, x);  //先获取value_后加x,value_值改变，返回之前获取的值
  }

  T addAndGet(T x)    // 加x 返回加x后的值
  {
    return getAndAdd(x) + x;
  }

  T incrementAndGet()   //加1 返回加1后的值
  {
    return addAndGet(1);
  }

  T decrementAndGet()  //减1 返回减1后的值
  {
    return addAndGet(-1);
  }

  void add(T x)   // 对象直接加x 不返回值
  {
    getAndAdd(x);
  }

  void increment()  //对象加一
  {
    incrementAndGet();
  }

  void decrement()  //对象减一
  {
    decrementAndGet();
  }

  T getAndSet(T newValue)  // 设置成某一个值
  {
    // in gcc >= 4.7: __atomic_exchange_n(&value, newValue, __ATOMIC_SEQ_CST)
    /*
    type __sync_lock_test_and_set (type *ptr, type value, ...) 将*ptr设为value并返回*ptr操作之前的值。
    void __sync_lock_release (type *ptr, ...)　 将*ptr置0
    */
    return __sync_lock_test_and_set(&value_, newValue);
  }
 private:
  volatile T value_;   // 防止编译器优化，不取寄存器的备份，每次都取内存中的值
};  // AtomicIntegerT

}  // namespace muduo::detail

//提供两个原子操作的int32 和 int64 的类
typedef detail::AtomicIntegerT<int32_t>  AtomicInt32;
typedef detail::AtomicIntegerT<int64_t>  AtomicInt64;

}  // namspace muduo

#endif  // MUDUO_BASE_ATOMIC_H
