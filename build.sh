#!/bin/sh

set -e
mkdir -p build

clang src/linecount_avx512.c -Wall -Wpedantic -O3 -mavx512bw -o build/linecount_avx512
