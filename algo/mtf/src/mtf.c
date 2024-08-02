/**
 * @file mtf.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of the Move-To-Front compression algorithm.
 * This file implements the MTF functions defined in mtf.h.
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

#include "../include/mtf.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Main function for the Move-to-Front (MTF) algorithm.
 * This function is used to encode or decode a file using the MTF algorithm.
 *
 * @param filename The relative or absolute path to the file.
 * @param mode Accepts only two values:
 *     - 'e' for encoding
 *     - 'd' for decoding
 * @return 0 upon success, otherwise, an error code.
 */
int mtf_file(const char *filename, const char mode)
{
    if (filename == NULL || (mode != 'e' && mode != 'd'))
    {
        fprintf(stderr, "Error: invalid arguments\n");
        return NULL_ERROR;
    }

    switch (mode)
    {
    case 'e':
        printf("(MTF) Encoding file: %s\n", filename);
        mtf_t mtf;
        mtf_init(&mtf, 255);
        mtf_enc_file(&mtf, filename);
        break;
    case 'd':
        printf("(MTF) Decoding file: %s\n", filename);
        mtf_t mtf2;
        mtf_init(&mtf2, 255);
        mtf_dec_file(&mtf2, filename);
        break;
    default:
        fprintf(stderr, "Error: invalid mode %c\n", mode);
        return VALUE_ERROR;
        break;
    }

    return 0;
}