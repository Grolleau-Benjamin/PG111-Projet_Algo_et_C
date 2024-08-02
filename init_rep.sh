#!/bin/bash

mkdir -p bin lib
mkdir -p algo/binio/obj algo/binio/bin
mkdir -p algo/huffman/obj algo/huffman/bin
mkdir -p algo/mtf/obj algo/mtf/bin

echo -e 'exec on bash ==> \n\texport LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(pwd)/lib"'
