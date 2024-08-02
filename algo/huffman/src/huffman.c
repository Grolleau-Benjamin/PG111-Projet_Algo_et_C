/**
 * @file huffman.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of Huffman encoding and decoding.
 * This file implements the huffman functions defined in huffman.h.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
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

#include "../include/huffman.h"
#include "../include/huffman_enc.h"
#include "../include/huffman_dec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Encodes a file using Huffman encoding.
 *
 * @param input_file Path to the input file.
 * @param output_file Path to the output file where the encoded message will be written.
 */
void huffman_encode(char *input_file, char *output_file)
{
    huff_encode(input_file, output_file);
    return;
}

/**
 * @brief Decodes a file using Huffman decoding.
 *
 * @param input_file Path to the input file containing the encoded message.
 * @param output_file Path to the output file where the decoded message will be written.
 */
void huffman_decode(char *input_file, char *output_file)
{
    huff_decode(input_file, output_file);
}

/**
 * @brief Main function for the Huffman algorithm.
 * This function is used to encode or decode a file using the MTF algorithm.
 *
 * @param filename The relative or absolute path to the file.
 * @param mode Accepts only two values:
 *     - 'e' for encoding
 *     - 'd' for decoding
 * @return 0 upon success, otherwise, an error code.
 */
int huffman_file(char *filename, const char mode)
{
    if (filename == NULL || (mode != 'e' && mode != 'd'))
    {
        fprintf(stderr, "Error: invalid arguments\n");
        return NULL_ERROR;
    }

    switch (mode)
    {
    case 'e':
    {
        size_t len_enc = strlen(filename) + strlen(HUFF_OUTPUT_FILE_ENC_SUFFIX) + 1;
        char *fout_name_enc = (char *)malloc(len_enc);
        if (fout_name_enc == NULL)
        {
            fprintf(stderr, "Error: malloc failed\n");
            return MEMORY_ERROR;
        }
        sprintf(fout_name_enc, "%s%s", filename, HUFF_OUTPUT_FILE_ENC_SUFFIX);
        printf("(Huffman) Encoding file: %s\n", filename);
        huffman_encode(filename, fout_name_enc);
        break;
    }
    case 'd':
    {
        size_t len_dec = strlen(filename) + strlen(HUFF_OUTPUT_FILE_DEC_SUFFIX) + 1;
        char *fout_name_dec = (char *)malloc(len_dec);
        if (fout_name_dec == NULL)
        {
            fprintf(stderr, "Error: malloc failed\n");
            return MEMORY_ERROR;
        }
        sprintf(fout_name_dec, "%s%s", filename, HUFF_OUTPUT_FILE_ENC_SUFFIX);
        printf("(Huffman) Decoding file: %s\n", filename);
        huffman_decode(filename, fout_name_dec);
        break;
    }
    default:
    {
        fprintf(stderr, "Error: invalid mode %c\n", mode);
        return VALUE_ERROR;
        break;
    }
    }

    return 0;
}