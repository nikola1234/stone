#!/bin/sh

set -x

g++ -g quicksort.cc -std=c++11 -o quicksort && ./quicksort