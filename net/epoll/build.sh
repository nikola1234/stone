#!/bin/sh

set -x

g++ -g my_client.cc -std=c++11 -o my_client
g++ -g my_server.cc -std=c++11 -o my_server
