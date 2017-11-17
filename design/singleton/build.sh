#!/bin/sh

set -x
g++ singleton.cc -std=c++11 -o singleton && ./singleton