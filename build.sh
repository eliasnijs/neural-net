#!/bin/sh

WFLAGS="-Wall -Wextra -Wpedantic -Wno-write-strings -Wno-unused-function"
CPPFLAGS="-std=c++11 -O3 -fsanitize=address -g"
g++ -o a.out $CPPFLAGS $WFLAGS main.cpp -lm

