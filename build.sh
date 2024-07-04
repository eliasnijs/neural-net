#!/bin/sh

WFLAGS="-Wall -Wextra -Wpedantic -Wno-write-strings -Wno-unused-function"
CPPFLAGS="-std=c++11 -O3 -fsanitize=address -g"
g++ -o build/a.out $CPPFLAGS $WFLAGS src/main.cpp -lm

