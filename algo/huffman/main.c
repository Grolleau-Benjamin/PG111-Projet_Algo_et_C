/**
 * @file main.c
 * @author bgrolleau001 llunet001
 * @brief Simple example of how to use the Huffman compression
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 */

#include <stdio.h>

#include "./include/huffman.h"

int main(int argc, char *argv[])
{
    huffman_encode("tests/input", "tests/encoded");
    huffman_decode("tests/encoded", "tests/decoded");
}