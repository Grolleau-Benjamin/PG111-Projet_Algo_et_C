/**
 * @file main.c
 * @brief Main program for the compression algorithms.
 */

/*
 * Copyright 2024 Benjamin Grolleau et Louis Lunet
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>

#include "./algo/mtf/include/mtf.h"
#include "./algo/huffman/include/huffman.h"
#include "./algo/lz/include/lz.h"

void help()
{
    printf("Usage: main <algo>\n");
    printf("Algo: m - Move-To-Front\n");
    printf("      h - Huffman\n");
    printf("      l - Lempel-Ziv\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        help();
        return 1;
    }

    switch (argv[1][0])
    {
    case 'm':
        mtf_file("./data/input", 'e');
        mtf_file("./data/input_MTFenc", 'd');
        break;
    case 'h':
        huffman_file("./data/input", 'e');
        huffman_file("./data/input_HUFFenc", 'd');
        break;
    case 'l':
        lz_file("./data/input", 'e');
        lz_file("./data/input_LZenc", 'd');
        break;
    default:
        help();
        break;
    }

    return 0;
}
