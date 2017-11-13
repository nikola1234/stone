#!/bin/sh

set -x
#用于脚本调试。set是把它下面的命令打印到屏幕
#set -x 是开启 set +x是关闭 set -o是查看 (xtrace)，set去追中一段代码的显示情况。 
g++ iter_no_use.cc -std=c++11 -o iter_no_use && ./iter_no_use