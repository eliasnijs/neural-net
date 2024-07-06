#!/bin/sh

WFLAGS="-Wall -Wextra -Wpedantic -Wno-write-strings -Wno-unused-function"
CPPFLAGS="-std=c++11 -O3 -g -fsanitize=address"

g++ -o build/a.out $CPPFLAGS $WFLAGS src/main.cpp -lm -fopenmp

