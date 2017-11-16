#!/bin/sh

set -x

# -c 只编译不连接
g++ -g app.cc -std=c++11 -o pvd && ./pvd