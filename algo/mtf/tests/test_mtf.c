/**
 * @file test_mtf.c
 * @author bgrolleau001 llunet001
 * @brief Test file for the the Move-To-Front compression algorithm.
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
#include <assert.h>
#include <stdio.h>

/**
 * @brief Test the mtf_init function.
 * It should initialize the mtf structure.
 *
 * @return Should panic if the test fails.
 */
void test_init()
{
  mtf_t mtf;
  mtf_init(&mtf, 256);
  assert(&mtf != NULL);
  for (int i = 0; i < 256; i++)
    assert(mtf.arr[i] == (char)i);
}

/**
 * @brief Test the mtf_enc_char function.
 * It should encode a character using the MTF algorithm.
 *
 * @return Should panic if the test fails.
 */
void test_enc_char_NULL_ERROR()
{
  mtf_t mtf;
  mtf_init(&mtf, 256);
  char c = 'a';
  assert(mtf_enc_char(NULL, c) == NULL_ERROR);
  fprintf(stderr, "==>this error is not to be taken in account, this error means test is successful\n");
  assert(mtf_enc_char(&mtf, c) != NULL_ERROR);
}

/**
 * @brief Test the mtf_enc_char function.
 * It should encode a character using the MTF algorithm.
 *
 * @return Should panic if the test fails.
 */
void test_enc_char_VALUE_ERROR()
{
  mtf_t mtf;
  mtf_init(&mtf, 200);
  assert(mtf_enc_char(&mtf, 250) == VALUE_ERROR);
  fprintf(stderr, "==>this error is not to be taken in account, this error means test is successful\n");
  assert(mtf_enc_char(&mtf, 150) != VALUE_ERROR);
}

/**
 * @brief Test the mtf_enc_char function.
 * It test if a result is equal to the expected output.
 *
 * @param a The input array.
 * @param b The expected output array.
 * @param n The size of the input and output arrays.
 *
 * @return Should panic if the test fails.
 */
void excpected_inout(unsigned char *a, unsigned char *b, int n)
{
  int i;
  mtf_t mtf;
  mtf_init(&mtf, 256);
  for (i = 0; i < n; ++i)
  {
    assert(mtf_enc_char(&mtf, a[i]) == b[i]);
  }
}

/**
 * @brief Test the mtf_enc_char function.
 * It should encode a character using the MTF algorithm.
 *
 * @note test for 3 modifications of the same character. ('a', 'a', 'a')
 */
void test_enc_char1()
{
  mtf_t mtf;
  mtf_init(&mtf, 256);
  assert(mtf_enc_char(&mtf, 'a') == 97);
  assert(mtf_enc_char(&mtf, 'a') == 0);
  assert(mtf_enc_char(&mtf, 'a') == 0);
}

/**
 * @brief Test the mtf_enc_char function.
 * It should encode 1 characters repeted 3 times using the MTF algorithm.
 * @note the character 'a' is represented by 97 in ASCII.
 * @note test the pattern ('a', 'a', 'a') qui doit donner (97, 0, 0)
 */
void test_enc_char2()
{
  unsigned char in[] = {'a', 'a', 'a'};
  unsigned char out[] = {97, 0, 0};
  excpected_inout(in, out, sizeof(in) / sizeof(in[0]));
}

/**
 * @brief Test the mtf_enc_char function.
 * It should encode two characters using the MTF algorithm.
 * @note test the pattern ('a', 'a', 'b', 'b', 'a') qui doit donner (97, 0, 98, 0, 1)
 */
void test_enc_char3()
{
  unsigned char in[] = {'a', 'a', 'b', 'b', 'a'};
  unsigned char out[] = {97, 0, 98, 0, 1};
  excpected_inout(in, out, sizeof(in) / sizeof(in[0]));
}

/**
 * @brief Test the mtf_enc_char function.
 * It should encode two characters using the MTF algorithm.
 * @note test the pattern ('a', 'b', 'r', 'a', 'c') qui doit donner (97, 98, 114, 2, 100)
 */
void test_enc_char4()
{
  unsigned char in[] = {97, 98, 114, 97, 99};
  unsigned char out[] = {97, 98, 114, 2, 100};
  excpected_inout(in, out, sizeof(in) / sizeof(in[0]));
}

/**
 * @brief Main function for the test_mtf program.
 */
int main()
{
  test_init();
  test_enc_char_NULL_ERROR();
  test_enc_char_VALUE_ERROR();
  test_enc_char1();
  test_enc_char2();
  test_enc_char3();
  test_enc_char4();
  printf("All tests passed successfully.\n");
}
