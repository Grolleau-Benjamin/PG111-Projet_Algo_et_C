/**
 * @file main.c
 * @author bgrolleau001 llunet001
 * @brief main file exemple for the binary stream I/O operations.
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


#include "include/binio.h"
#include <stdio.h>

int main() {
    #ifdef DEBUG
        printf("DEBUG is defined\n");
    #endif

    BFILE *input = bopen("tests/input", 'r', 1);
    if (input == NULL) {
        printf("Error: Could not open the file.\n");
        return 1;
    }

    // ***************************** Test for bgetbit
    int c, i = 1;
    while ((c = bgetbit(input)) != EOF) {
        DEBUG_PRINT("\t[+] bstream->buffer  = %d (0x%X)\n", input->buffer, input->buffer);
        DEBUG_PRINT("\t[+] bstream->bit_pos = %d\n", input->bit_pos);
        DEBUG_PRINT("\t[+] bit value        = %d\n", c);
        #ifndef DEBUG 
            printf("%d", c);
            if (i % 8 == 0) {
                printf(" ");
            }
        #endif
        i++;
    }
    printf("\n");

    // ************************** Test for bread
    unsigned int nbits = 16;
    unsigned char buffer[2] = {0x00, 0x00};
    unsigned int readen = bread(buffer, nbits, input);
    DEBUG_PRINT("\t[+] Readen: %d\n", readen);
    DEBUG_PRINT("\t[+] Buffer: 0x%X 0x%X\n", buffer[0], buffer[1]);

    bclose(input);

    BFILE *output = bopen("tests/output", 'w', 1);
    if (output == NULL) {
        printf("Error: Could not open the file.\n");
        return 1;
    }

    // ************************** Test for bputbit
    bputbit(1, output);
    bputbit(0, output);
    bputbit(1, output);
    bputbit(0, output);
    bputbit(1, output);
    bputbit(1, output);
    bputbit(1, output);
    bputbit(0, output);
    bputbit(1, output);

    bclose(output);

    input = bopen("tests/input", 'r', 1);
    if (input == NULL) {
        printf("Error: Could not open the file.\n");
        return 1;
    }

    output = bopen("tests/output", 'w', 1);
    if (output == NULL) {
        printf("Error: Could not open the file.\n");
        return 1;
    }

    readen = bread(buffer, nbits, input);
    printf("\t[+] Readen: %d\n", readen);
    printf("\t[+] Buffer: 0x%X 0x%X\n", buffer[0], buffer[1]);
    unsigned int written = bwrite(buffer, readen, output);
    printf("\t[+] Written: %d\n", written);

    bclose(input);
    bclose(output);

    return 0;
}
