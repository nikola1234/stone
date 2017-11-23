#!/bin/sh

set -x
g++ decorator.cc -std=c++11 -o decorator && ./decorator