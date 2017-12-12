#ifndef MUDUO_BASE_NONCOPYABLE_H
#define MUDUO_BASE_NONCOPYABLE_H

namespace muduo
{
// 标签类 不允许类 实现一个对象，不允许派生类对象 拷贝构造、赋值构造另一个对象
class noncopyable
{
 protected:
  noncopyable() = default;
  ~noncopyable() = default;

 private:
  noncopyable(const noncopyable&) = delete;             // 拷贝构造函数  禁止 a(b)
  noncopyable& operator=(const noncopyable&) = delete;  // 赋值构造函数  禁止 a=b
  noncopyable(noncopyable&&) = delete;                  // 防止右值引用作为参数
  noncopyable& operator=(noncopyable&&) = delete;
};

}

#endif  // MUDUO_BASE_NONCOPYABLE_H
