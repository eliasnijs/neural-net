#!/bin/sh

WFLAGS="-Wall -Wextra -Wpedantic -Wno-write-strings -Wno-unused-function"
CPPFLAGS="-std=c++11 -O3 -fsanitize=address -g"
g++ $CPPFLAGS $WFLAGS micrograd.cpp -lm -lgvc -lcgraph -lcdt -o a.out

