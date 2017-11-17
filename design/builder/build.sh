#!/bin/sh

set -x
g++ builder.cc -std=c++11 -o builder && ./builder