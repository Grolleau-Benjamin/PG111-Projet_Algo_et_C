/**
 * @file test_lz.c
 * @author bgrolleau001 llunet001
 * @brief Test file for the Lempel-Ziv compression algorithm.
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


#include "../include/lz.h"
#include <assert.h>
#include <stdio.h>

/**
 * @brief Test the lz_create_node function.
 * It should create a new tree node with the specified key and value.
 * 
 * @return Should panic if the test fails.
*/
void test_create_node() {
    TreeNode *node = lz_create_node("a", 1);
    assert(node != NULL);
    assert(strcmp(node->key, "a") == 0);
    assert(node->value == 1);
    assert(node->left == NULL);
    assert(node->right == NULL);
    free(node);
}

/**
 * @brief Test the lz_insert_node function.
 * It should insert a new node into the tree.
 * 
 * @return Should panic if the test fails.
*/
void test_insert_node() {
    TreeNode *root = lz_create_node("m", 1);
    root = lz_insert_node(root, "a", 2);
    root = lz_insert_node(root, "z", 3);

    TreeNode *node = lz_search_node(root, "a");
    assert(node != NULL);
    assert(strcmp(node->key, "a") == 0);
    assert(node->value == 2);

    node = lz_search_node(root, "z");
    assert(node != NULL);
    assert(strcmp(node->key, "z") == 0);
    assert(node->value == 3);

    lz_free_tree(root);
}

/**
 * @brief Test the lz_search_node function.
 * It should search for a node with the specified key in the tree.
 * 
 * @return Should panic if the test fails.
*/
void test_search_node() {
    TreeNode *root = lz_create_node("m", 1);
    root = lz_insert_node(root, "a", 2);
    root = lz_insert_node(root, "z", 3);

    TreeNode *node = lz_search_node(root, "a");
    assert(node != NULL);
    assert(strcmp(node->key, "a") == 0);
    assert(node->value == 2);

    node = lz_search_node(root, "z");
    assert(node != NULL);
    assert(strcmp(node->key, "z") == 0);
    assert(node->value == 3);

    node = lz_search_node(root, "x");
    assert(node == NULL);

    lz_free_tree(root);
}

/**
 * @brief Test the lz_encoding function.
 * It should encode a file using the LZ algorithm.
 * 
 * @return Should panic if the test fails.
*/
void test_encoding() {
    const char *input_filename = "test_input.txt";
    const char *output_filename = "test_input.txt_LZenc";

    FILE *input_file = fopen(input_filename, "w");
    fprintf(input_file, "abracadabra");
    fclose(input_file);

    lz_encoding(input_filename, output_filename);

    FILE *output_file = fopen(output_filename, "r");
    assert(output_file != NULL);

    int code;
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 97);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 98);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 114);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 97);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 99);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 97);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 100);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 256);
    assert(fscanf(output_file, "%d", &code) == 1);
    assert(code == 258);

    fclose(output_file);
    remove(input_filename);
    remove(output_filename);
}

/**
 * @brief Test the lz_decoding function.
 * It should decode a file using the LZ algorithm.
 * 
 * @return Should panic if the test fails.
*/
void test_decoding() {
    const char *input_filename = "test_input.txt_LZenc";
    const char *output_filename = "test_input.txt_LZdec";

    FILE *input_file = fopen(input_filename, "w");
    fprintf(input_file, "97 98 114 97 99 97 100 256 258");
    fclose(input_file);

    lz_decoding(input_filename, output_filename);

    FILE *output_file = fopen(output_filename, "r");
    assert(output_file != NULL);

    char decoded[20];
    assert(fgets(decoded, sizeof(decoded), output_file) != NULL);
    assert(strcmp(decoded, "abracadabra") == 0);

    fclose(output_file);
    remove(input_filename);
    remove(output_filename);
}

/**
 * @brief Main function for the test_lz program.
*/
int main() {
    test_create_node();
    test_insert_node();
    test_search_node();
    test_encoding();
    test_decoding();

    printf("All tests passed successfully.\n");
    return 0;
}
