/**
 * @file test_binio.c
 * @author bgrolleau001 llunet001
 * @brief Test file for the binary I/O functions.
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


#include "../include/binio.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Test the bopen function.
 * It should open a binary file for reading or writing.
 * 
 * @return Should panic if the test fails.
*/
void test_bopen() {
    BFILE *bfile = bopen("test_bopen.bin", 'w', 0);
    assert(bfile != NULL);
    assert(bfile->file != NULL);
    assert(bfile->mode == 'w');
    assert(bfile->padding == 0);
    bclose(bfile);

    bfile = bopen("test_bopen.bin", 'r', 0);
    assert(bfile != NULL);
    assert(bfile->file != NULL);
    assert(bfile->mode == 'r');
    assert(bfile->padding == 0);
    bclose(bfile);

    bfile = bopen("test_bopen.bin", 'w', 1);
    assert(bfile != NULL);
    assert(bfile->file != NULL);
    assert(bfile->mode == 'w');
    assert(bfile->padding == 1);
    bclose(bfile);

    bfile = bopen("test_bopen.bin", 'r', 1);
    assert(bfile != NULL);
    assert(bfile->file != NULL);
    assert(bfile->mode == 'r');
    assert(bfile->padding == 1);
    bclose(bfile);

    printf("All open tests passed successfully.\n");

    remove("test_bopen.bin");
}

/**
 * @brief Test the bclose function.
 * It should close a binary file and free the binary descriptor.
 * 
 * @return Should panic if the test fails.
*/
void test_bclose() {
    BFILE *bfile = bopen("test_bclose.bin", 'w', 0);
    assert(bfile != NULL);
    assert(bclose(bfile) == 0);
    remove("test_bclose.bin");

    printf("All close tests passed successfully.\n");
}

/**
 * @brief Test the bgetbit function.
 * It should read individual bits from a binary file.
 * 
 * @return Should panic if the test fails.
*/
void test_bgetbit() {
    BFILE *bfile = bopen("test_bgetbit.bin", 'w', 1);
    assert(bfile != NULL);

    unsigned char data = 0b11001100;
    assert(bwrite(&data, 8, bfile) == 8);
    unsigned char padding = 0b10000000;
    assert(bwrite(&padding, 8, bfile) == 8);
    bclose(bfile);

    bfile = bopen("test_bgetbit.bin", 'r', 1);
    assert(bgetbit(bfile) == 0);
    assert(bgetbit(bfile) == 0);
    assert(bgetbit(bfile) == 1);
    assert(bgetbit(bfile) == 1);
    assert(bgetbit(bfile) == 0);
    assert(bgetbit(bfile) == 0);
    assert(bgetbit(bfile) == 1);
    assert(bgetbit(bfile) == 1);
    bclose(bfile);

    remove("test_bgetbit.bin");
    printf("All getbit tests passed successfully.\n");
}

/**
 * @brief Test the bputbit function.
 * It should write individual bits to a binary file.
 * 
 * @return Should panic if the test fails.
*/
void test_bputbit() {
    BFILE *bfile = bopen("test_bputbit.bin", 'w', 1);
    assert(bfile != NULL);

    bputbit(1, bfile);
    bputbit(1, bfile);
    bputbit(0, bfile);
    bputbit(0, bfile);
    bputbit(1, bfile);
    bputbit(1, bfile);
    bputbit(0, bfile);
    bputbit(0, bfile);
    bclose(bfile);

    // 1100 1100
    // 1100 1___ => padding

    bfile = bopen("test_bputbit.bin", 'r', 1);
    unsigned char read_data = 0;
    assert(bread(&read_data, 8, bfile) == 5);
    assert(read_data == 0b00010011);  // !!! lecture a l'envers       1010 => 0101
    bclose(bfile);

    remove("test_bputbit.bin");
    printf("All putbit tests passed successfully.\n");
}

/**
 * @brief Test the bwrite function.
 * It should write bits to a binary file.
 * 
 * @return Should panic if the test fails.
*/
void test_bwrite() {
    BFILE *bfile = bopen("test_bwrite.bin", 'w', 1);
    assert(bfile != NULL);

    unsigned char data = 0b11001100;
    assert(bwrite(&data, 8, bfile) == 8);
    unsigned char padding = 0b10000000;
    assert(bwrite(&padding, 8, bfile) == 8);
    bclose(bfile);

    bfile = bopen("test_bwrite.bin", 'r', 1);
    unsigned char read_data = 0;
    assert(bread(&read_data, 8, bfile) == 8);
    assert(read_data == 0b11001100);
    assert(bread(&read_data, 8, bfile) == 8);
    assert(read_data == 0b10000000);
    bclose(bfile);

    remove("test_bwrite.bin");
    printf("All write tests passed successfully.\n");
}

/**
 * @brief Test the bread function.
 * It should read bits from a binary file.
 * 
 * @return Should panic if the test fails.
*/
void test_bread() {
    BFILE *bfile = bopen("test_bread.bin", 'w', 1);
    assert(bfile != NULL);

    unsigned char data = 0b11001100;
    assert(bwrite(&data, 8, bfile) == 8);
    unsigned char padding = 0b10000000;
    assert(bwrite(&padding, 8, bfile) == 8);
    bclose(bfile);

    bfile = bopen("test_bread.bin", 'r', 1);
    unsigned char read_data = 0;
    assert(bread(&read_data, 8, bfile) == 8);
    assert(read_data == 0b11001100);
    assert(bread(&read_data, 8, bfile) == 8);
    assert(read_data == 0b10000000);
    bclose(bfile);

    remove("test_bread.bin");
    printf("All read tests passed successfully.\n");
}

/**
 * @brief Main function for the test_binio program.
*/
int main() {
    test_bopen();
    test_bclose();
    test_bgetbit();
    test_bputbit();

    printf("All tests passed successfully.\n");
    return 0;
}
