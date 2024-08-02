/**
 * @file main.c
 * @author bgrolleau001 llunet001
 * @brief Simple example of how to use the Lempel-Ziv compression algorithm.
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
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


#include "./include/lz.h"

int main() {
    const char *input_string = "tests/input.txt";
    const char *encoded_file = "tests/encoded.txt";
    const char *decoded_file = "tests/decoded.txt";

    lz_encoding(input_string, encoded_file);
    lz_decoding(encoded_file, decoded_file);

    return 0;
}
