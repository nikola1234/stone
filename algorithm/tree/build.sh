#!/bin/sh

set -x

# -c 只编译不连接
g++ -g tree.cc -std=c++11 -o tree && ./tree