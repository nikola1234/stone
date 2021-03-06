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

  void swap(Timestamp& that)
  {
    std::swap(this->microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
  }

  string toString() const;

  string toFormattedString(bool showMicroseconds = true) const;

  bool valid() const { return microSecondsSinceEpoch_ > 0; }  // 时间有效性检查

  // for internal usage. 一个us时间  一个s时间 返回的类型也不一样
  int64_t microSecondsSinceEpoch() const { return microSecondsSinceEpoch_; }
  time_t secondsSinceEpoch() const
  { return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond); }


  static Timestamp now();     // 构造时间的静态函数 Get time of now.
  static Timestamp invalid()  // 构造一个无效(microSecondsSinceEpoch_ = 0)的时间对象
  {
    return Timestamp();
  }

  // 从unix时间类型(time_t)转换成 Timestamp时间类型
  static Timestamp fromUnixTime(time_t t)
  {
    return fromUnixTime(t, 0);
  }

  static Timestamp fromUnixTime(time_t t, int microseconds)
  {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
  }

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

///
/// Gets time difference of two timestamps, result in seconds.
///
/// @param high, low
/// @return (high-low) in seconds
/// @c double has 52-bit precision, enough for one-microsecond
/// resolution for next 100 years.
inline double timeDifference(Timestamp high, Timestamp low)
{
  int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

///
/// Add @c seconds to given timestamp.
///
/// @return timestamp+seconds as Timestamp
///
inline Timestamp addTime(Timestamp timestamp, double seconds)
{
  int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.microSecondsSinceEpoch() + delta);
}
    
} /*end namespace moduo */
#endif    // MUDUO_BASE_TIMESTAMP_H
