#include <muduo/base/Timestamp.h>

#include <sys/time.h>
#include <stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS  // PRId64定义
#endif

#include <inttypes.h>

using namespace muduo;

static_assert(sizeof(Timestamp) == sizeof(int64_t),
              "Timestamp is same size as int64_t");  // 编译时检查

string Timestamp::toString() const
{
  char buf[32] = {0};
  int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;       // s
  int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;  // us 
  //  PRId64 在32位系统上是“lld”   在64位系统上是“ld” 字符串
  //  snprintf(char *str, size_t size, const char *format, ...)。
  snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
  return buf;
}

string Timestamp::toFormattedString(bool showMicroseconds) const
{
  char buf[32] = {0};
  // time_t 这种类型就是用来存储从1970年到现在经过了多少秒
  //要想更精确一点，可以用结构struct timeval，它精确到微妙。
  //struct timeval
  //{
  //  long tv_sec; /*秒*/
  //  long tv_usec; /*微秒*/
  //};
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
//struct tm
//{
//    int tm_sec;  /*秒，正常范围0-59， 但允许至61*/
//    int tm_min;  /*分钟，0-59*/
//    int tm_hour; /*小时， 0-23*/
//    int tm_mday; /*日，即一个月中的第几天，1-31*/
//    int tm_mon;  /*月， 从一月算起，0-11*/  1+p->tm_mon;
//    int tm_year;  /*年， 从1900至今已经多少年*/  1900＋ p->tm_year;
//    int tm_wday; /*星期，一周中的第几天， 从星期日算起，0-6*/
//    int tm_yday; /*从今年1月1日到目前的天数，范围0-365*/
//    int tm_isdst; /*日光节约时间的旗标*/
//};
  struct tm tm_time;
//#include<time.h>
//struct tm *gmtime(const time_t *timep)；
//struct tm *gmtime_r(const time_t *timep, struct tm *result);
//gmtime返回的是一个structtm*，这个指针指向一个静态的内存，这块区域是会经常被改动的
  gmtime_r(&seconds, &tm_time);  // 线程安全的函数

  if (showMicroseconds)
  {
    int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
  }
  else
  {
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
  }
  return buf;
}

Timestamp Timestamp::now()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  int64_t seconds = tv.tv_sec;
  return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}