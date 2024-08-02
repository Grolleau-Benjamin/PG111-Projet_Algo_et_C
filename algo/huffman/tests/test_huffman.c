/**
 * @file test_huffman.c
 * @author bgrolleau001 llunet001
 * @brief Test file for the Huffman compression algorithm.
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
#include <assert.h>
#include "huffman_enc.h"
#include "huffman_dec.h"

void test_frequency_tab()
{
    printf("Testing frequency tab functions:\n");

    struct frequency_tab ft;
    init_frequency_tab(&ft);

    // Update frequency table with sample data
    update_frequency_tab(&ft, "tests/input");

    // Verify frequencies
    assert(ft.arr['a'] == 3);
    assert(ft.arr['b'] == 1);
    assert(ft.arr['c'] == 2);
    assert(ft.arr['d'] == 1);

    printf("Frequency tab test passed!\n\n");
}

void test_build_tree()
{
    printf("Testing build tree function:\n");

    struct frequency_tab ft;
    init_frequency_tab(&ft);
    update_frequency_tab(&ft, "tests/input");

    int leaf_count = 0;
    for (int i = 0; i < ft.size; i++)
    {
        if (ft.arr[i] > 0)
        {
            leaf_count++;
        }
    }

    struct leaf *leaves = (struct leaf *)malloc(leaf_count * sizeof(struct leaf));
    struct node *nodes = (struct node *)malloc(leaf_count * sizeof(struct node));

    struct node *tree = build_tree(&ft, leaves, nodes, leaf_count);

    // Traverse the tree and verify structure
    assert(tree->value == 7);
    assert(tree->l->value == 3);
    assert(tree->r->value == 4);
    assert(tree->r->l->value == 2);
    assert(tree->r->r->value == 2);

    printf("Build tree test passed!\n\n");
}

void test_encode_decode()
{
    printf("Testing encode and decode functions:\n");

    char *input_file = "tests/input";
    char *encoded_file = "tests/sample_encoded";
    char *decoded_file = "tests/sample_decoded";

    // Encode file
    huff_encode(input_file, encoded_file);

    // Decode file
    huff_decode(encoded_file, decoded_file);

    // Verify decoded content
    FILE *input = fopen(input_file, "r");
    FILE *decoded = fopen(decoded_file, "r");
    assert(input != NULL && decoded != NULL);
    int ch1, ch2;
    while ((ch1 = fgetc(input)) != EOF && (ch2 = fgetc(decoded)) != EOF)
    {
        assert(ch1 == ch2);
    }

    fclose(input);
    fclose(decoded);

    printf("Encode and decode test passed!\n\n");
}

int main()
{
    test_frequency_tab();
    test_build_tree();

    test_encode_decode();

    printf("All unit tests passed!\n");

    return 0;
}
