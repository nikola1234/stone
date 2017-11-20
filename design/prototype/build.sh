#!/bin/sh

set -x
g++ prototype.cc -std=c++11 -o prototype && ./prototype