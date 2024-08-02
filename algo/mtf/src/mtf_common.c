/**
 * @file mtf_common.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of the Move-To-Front common functions.
 * This file implements the common MTF functions defined in mtf_common.h.
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

#include "../include/mtf_common.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Initialize the MTF struct for decoding
 * @param mtf_t *mtf: pointer to the MTF struct
 * @param int size: size of the MTF array (default 255)
 * @return 0 on success, -1 on failure
 */
int mtf_init(mtf_t *mtf, unsigned int size)
{
    DEBUG_PRINT("[DEBUG] (MTF) mtf_dec_init(mtf_t *%p, int %d);\n", mtf, size);
    if (mtf == NULL)
    {
        fprintf(stderr, "Error: mtf_dec_init() received NULL pointer\n");
        return NULL_ERROR;
    }
    if (size < 0 || size > 256)
    {
        fprintf(stderr, "Error: mtf_dec_init() received invalid size %d\n", size);
        return VALUE_ERROR;
    }

    mtf->size = size;

    for (int i = 0; i < size; i++)
    {
        mtf->arr[i] = i;
    }
    return 0;
}