#ifndef MUDUO_BASE_COPYABLE_H
#define MUDUO_BASE_COPYABLE_H

namespace muduo
{
//标签类，只是表示派生类可以拷贝赋值
//派生类是一个值的类
class copyable
{
 //不想Copyable实例化
 protected:
  copyable() = default;
  ~copyable() = default;
};
}

#endif