#!/bin/sh

set -x
g++ assert_use.cc -std=c++11 -DNDEBUG -DCHECK_PTHREAD_RETURN_VALUE -o assert_use && ./assert_use
#g++ assert_use.cc -std=c++11 -DCHECK_PTHREAD_RETURN_VALUE -o assert_use && ./assert_use
#g++ assert_use.cc -std=c++11 -o assert_use && ./assert_use
