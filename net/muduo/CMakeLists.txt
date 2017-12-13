cmake_minimum_required(VERSION 2.6)

project(muduo C CXX)        #指定工程名称,并可指定工程支持的语言。支持语言列表可忽略,默认支持所有语言

enable_testing()            #增加测试，支持add_test
#add_test(NAME <name> [CONFIGURATIONS [Debug|Release|...]]
#                     [WORKING_DIRECTORY dir]
#                     COMMAND <command> [arg1 [arg2 ...]])

if(NOT CMAKE_BUILD_TYPE)    #设置编译类型 Debug or Release
    set(CMAKE_BUILD_TYPE "Release")
endif() 

set(CXX_FLAGS
 -g                         # 产生调试信息
 # -DVALGRIND               #
 # -DMUDUO_STD_STRING       #  string类的切换
 -DCHECK_PTHREAD_RETURN_VALUE #
 -D_FILE_OFFSET_BITS=64       #
 -Wall                      #输出大部分警告
 -Wextra                    #输出一些额外的警告
 -Werror                    #把警告当做错误，出现警告就停止编译
 -Wconversion               #隐式转换给出警告
 -Wno-unused-parameter      #当函数中出现未使用的参数时，不给出警告
 -Wold-style-cast           #C语言风格的转换给出警告
 -Woverloaded-virtual       # 当心虚函数重载，会影响多态的实现
 -Wpointer-arith            #当对函数指针和void*类型的指针进行数学运算给出警告
 -Wshadow                   #当一个局部变量遮盖住另一个局部变量，或者全局变量给出警告
 -Wwrite-strings            #规定字符串常量类型是const char[length],把地址赋值给non-const char*将产生警告，编译时发现尝试修改
                            #常量字符串的代码
 -march=native              #指定cpu体系的平台为本平台
 -std=c++0x                 #
 -rdynamic                  #它将指示连接器把所有符号（而不仅仅只是程序已使用到的外部符号）都添加到动态符号表
                            #（即.dynsym表）里，以便那些通过 dlopen() 或 backtrace() 
                            #（这一系列函数使用.dynsym表内符号）这样的函数使用。
 )

if(CMAKE_BUILD_BITS EQUAL 32)     # 32位系统
    list(APPEND CXX_FLAGS "-m32")
endif()

#string(REPLACE <match_string> <replace_string> <output variable> <input> [<input>...])
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")          # 编译器g++
set(CMAKE_CXX_FLAGS_DEBUG "-O0")       # debug版本  编译优化选项 -O0
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")  # 限制内联函数的大小
#cmake中一些预定义变量 PROJECT_BINARY_DIR 运行cmake命令的目录,通常是${PROJECT_SOURCE_DIR}/build
#PROJECT_SOURCE_DIR 工程的根目录 学习参考http://blog.csdn.net/wzzfeitian/article/details/40963457
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_package(Boost REQUIRED)
find_package(ZLIB)
find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)
find_library(BOOSTPO_LIBRARY NAMES boost_program_options)


if(ZLIB_FOUND)
  message(STATUS "found zlib")
endif()

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(muduo/base)


