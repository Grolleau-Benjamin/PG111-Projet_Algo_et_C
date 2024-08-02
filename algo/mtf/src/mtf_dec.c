/**
 * @file mtf_dec.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of the Move-To-Front decompression algorithm.
 * This file implements the MTF decoding functions defined in mtf_dec.h.
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

#include "../include/mtf_dec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Move the character at the given index to the front of the array and return the previous index of the character
 * @param mtf_t *mtf: pointer to the MTF struct
 * @param char c: the character to move
 * @return 0 on success, -1 on failure
 */
int mtf_dec_char(mtf_t *mtf, char c)
{
    DEBUG_PRINT("[DEBUG] (MTF) mtf_dec_char(mtf_t *%p, char %c);\n", mtf, c);
    if (mtf == NULL)
    {
        fprintf(stderr, "Error: mtf_dec_char() received NULL pointer\n");
        return NULL_ERROR;
    }

    int index = -1;
    for (int i = 0; i < mtf->size; i++)
    {
        if (mtf->arr[i] == c)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        fprintf(stderr, "Error: mtf_dec_char() character not found\n");
        return VALUE_ERROR;
    }

    char temp = mtf->arr[index];
    for (int i = index; i > 0; i--)
    {
        mtf->arr[i] = mtf->arr[i - 1];
    }
    mtf->arr[0] = temp;

    return index;
}

/**
 * @brief Decode the given file using MTF
 * @param mtf_t *mtf: pointer to the MTF struct
 * @param char *filename: the name of the file to decode
 * @return 0 on success, -1 on failure
 */
int mtf_dec_file(mtf_t *mtf, const char *filename)
{
    DEBUG_PRINT("[DEBUG] (MTF) mtf_dec_file(mtf_t *%p, const char *%s);\n", mtf, filename);
    if (mtf == NULL || filename == NULL)
    {
        fprintf(stderr, "Error: mtf_dec_file() received NULL pointer\n");
        return NULL_ERROR;
    }

    char *fout_name = (char *)malloc(strlen(filename) + sizeof(MTF_OUTPUT_FILE_DEC_SUFFIX));
    if (fout_name == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return MEMORY_ERROR;
    }
    sprintf(fout_name, "%s%s", filename, MTF_OUTPUT_FILE_DEC_SUFFIX);

    FILE *file = fopen(filename, "r");
    FILE *output = fopen(fout_name, "w");
    if (file == NULL || output == NULL)
    {
        if (file != NULL)
            fclose(file);
        if (output != NULL)
            fclose(output);
        return FILE_ERROR;
    }

    char c;
    while ((c = fgetc(file)) != EOF)
    {
        char actualChar = mtf->arr[(unsigned char)c];
        if (mtf_dec_char(mtf, actualChar) == -1)
        {
            fclose(file);
            fclose(output);
            return FILE_ERROR;
        }
        fputc(actualChar, output);
    }

    free(fout_name);
    fclose(file);
    fclose(output);
    return 0;
}