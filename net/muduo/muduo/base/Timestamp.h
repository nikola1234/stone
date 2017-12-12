#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

#include <boost/operators.hpp>

// moduo库命名空间
namespace muduo 
{
/// Time stamp in UTC, in microseconds resolution.
// time() returns the time as the number of seconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
// Epoch 纪元时间： 1970-01-01 00:00:00 +0000
// UTC  世界标准时间
// microseconds 微秒  milisecond 毫秒
class Timestamp : public muduo::copyable,
                  public boost::equality_comparable<Timestamp>,  // 需要实现 ==  ，自动实现 !=
                  public boost::less_than_comparable<Timestamp>  // 需要实现 <.自动实现 >, <=, >=
{
 public:
  Timestamp():microSecondsSinceEpoch_(0) {}

  explicit Timestamp(int64_t microSecondsSinceEpochArg) :
   microSecondsSinceEpoch_(microSecondsSinceEpochArg) {}

  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }

  void swap(Timestamp& that)
  {
    std::swap(this->microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  string toString() const;

  string toFormattedString(bool showMicroseconds = true) const;


  // google 定义标准 const变量名称之前加小写k字母  k谐音const
  static const int kMicroSecondsPerSecond = 1000 * 1000;  // s -> us  1000*1000

 private:
  int64_t microSecondsSinceEpoch_;  // 距离纪元时间的微秒数
  // int64_t 最大值 9223372036854775807 现在这个值大约  148219200000000
} /*end Timestamp*/;

inline bool operator<(Timestamp lhs, Timestamp rhs) 
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}
    
} /*end namespace moduo */
#endif    // MUDUO_BASE_TIMESTAMP_H
