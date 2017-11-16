#!/bin/sh

set -x
g++ static.cc -std=c++11 -o static && ./static
