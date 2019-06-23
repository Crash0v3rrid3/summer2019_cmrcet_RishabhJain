#!/bin/sh

g++ -g $(find -iname "*.cpp" 2>/dev/null) -o ./main
./main
