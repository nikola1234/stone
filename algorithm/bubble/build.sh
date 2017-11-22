#!/bin/sh

set -x

# -c 只编译不连接
g++ -g bubble.cc -std=c++11 -o bubble && ./bubble