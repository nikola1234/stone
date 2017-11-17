#!/bin/sh

set -x
g++ simplefactory.cc -std=c++11 -o simplefactory && ./simplefactory
g++ abstractfactory.cc -std=c++11 -o abstractfactory && ./abstractfactory