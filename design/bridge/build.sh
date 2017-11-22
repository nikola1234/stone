#!/bin/sh

set -x
g++ bridge.cc -std=c++11 -o bridge && ./bridge