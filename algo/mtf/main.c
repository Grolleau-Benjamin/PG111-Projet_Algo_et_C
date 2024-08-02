/**
 * @file main.c
 * @author bgrolleau001 llunet001
 * @brief Simple example of how to use the Move-To-Front compression algorithm.
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

#include <stdio.h>
#ifdef DEBUG
#include <assert.h>
#endif

#include "include/mtf.h"

int main(int argc, char *argv[])
{
    printf("Hello world from MTF main\n");

#ifdef DEBUG
    printf("Debugging\n");
#endif

    mtf_t mtf;
    mtf_init(&mtf, 256);

/* Execution du code */
#ifdef DEBUG
    // a is at index 97
    int result = mtf_enc_char(&mtf, 'a');
    assert(result == 'a');
    assert(mtf.arr[0] == 'a');
    // b is at index 98 and a is now at index 0
    int result2 = mtf_enc_char(&mtf, 'b');
    assert(result2 == 'b');
    assert(mtf.arr[0] == 'b');
    assert(mtf.arr[1] == 'a');
    // b is at index 0 and a is at index 1
    int result3 = mtf_enc_char(&mtf, 'b');
    assert(result3 == 0);
    assert(mtf.arr[0] == 'b');
    assert(mtf.arr[1] == 'a');
#endif

    mtf_init(&mtf, 256);
    mtf_enc_file(&mtf, "tests/input");

    mtf_t mtf2;
    mtf_init(&mtf2, 255);
    mtf_dec_file(&mtf2, "tests/input_MTFenc");

    printf("End of program, execution success\n");
    return 0;
}
