/**
 * @file mtf_enc.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of the Move-To-Front encoding algorithm.
 * This file implements the MTF encoding functions defined in mtf_enc.h.
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

#include "../include/mtf_enc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Move the character at the given index to the front of the array
 * and return the previous index of the character
 * @param mtf_t *mtf: pointer to the MTF struct
 * @param char c: the character to move
 * @return 0 on success, -1 on failure
 */
int mtf_enc_char(mtf_t *mtf, char c)
{
    DEBUG_PRINT("[DEBUG] (MTF) mtf_enc_char(mtf_t *%p, char %c);\n", mtf, c);
    if (mtf == NULL)
    {
        fprintf(stderr, "Error: mtf_enc_char() received NULL pointer\n");
        return NULL_ERROR;
    }
    /* SL: you could maintain the inverse of arr in the structure in order to get directly
     * the index of c
     */
    int i = 0;
    while (mtf->arr[i] != c && i < mtf->size)
    {
        i++;
    }
    if (i == mtf->size)
    {
        fprintf(stderr, "Error: mtf_enc_char() received invalid character %c\n", c);
        return VALUE_ERROR;
    }

    for (int j = i; j > 0; j--)
    {
        mtf->arr[j] = mtf->arr[j - 1];
    }
    mtf->arr[0] = c;

    return i;
}

/**
 * @brief Encode the given file using the MTF algorithm
 * @param mtf_t *mtf: pointer to the MTF struct
 * @param const char *filename: the name of the file to encode
 * @return 0 on success, -1 on failure
 */
int mtf_enc_file(mtf_t *mtf, const char *filename)
{
    DEBUG_PRINT("[DEBUG] (MTF) mtf_enc_file(mtf_t *%p, const char *%s);\n", mtf, filename);
    if (mtf == NULL || filename == NULL)
    {
        fprintf(stderr, "Error: mtf_enc_file() received NULL pointer\n");
        return NULL_ERROR;
    }

    char *fout_name = (char *)malloc(strlen(filename) + sizeof(MTF_OUTPUT_FILE_ENC_SUFFIX));
    if (fout_name == NULL)
    {
        fprintf(stderr, "Error: malloc failed\n");
        return MEMORY_ERROR;
    }
    sprintf(fout_name, "%s%s", filename, MTF_OUTPUT_FILE_ENC_SUFFIX);

    FILE *fin = fopen(filename, "r");
    FILE *fout = fopen(fout_name, "w");
    if (fin == NULL || fout == NULL)
    {
        if (fin != NULL)
            fclose(fin);
        if (fout != NULL)
            fclose(fout);
        return NULL_ERROR;
    }

    char chr;
    while ((chr = fgetc(fin)) != EOF)
    {
        int result = mtf_enc_char(mtf, chr);
        if (result == -1)
        {
            fclose(fin);
            fclose(fout);
            return FILE_ERROR;
        }
        fputc(result, fout);
    }

    fclose(fin);
    fclose(fout);
    free(fout_name);
    return 0;
}
