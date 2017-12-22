#ifndef MUDUO_BASE_LOGSTREAM_H
#define MUDUO_BASE_LOGSTREAM_H

#include <muduo/base/StringPiece.h>
#include <muduo/base/Types.h>
#include <assert.h>
#include <string.h> // memcpy
#ifndef MUDUO_STD_STRING
#include <string>
#endif

namespace muduo
{

namespace detail
{
}

class LogStream : noncopyable
{
};

class Fmt // : noncopyable
{
};

}
#endif  // MUDUO_BASE_LOGSTREAM_H