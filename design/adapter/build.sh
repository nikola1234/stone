#!/bin/sh

set -x
g++ adapter.cc -std=c++11 -o adapter && ./adapter