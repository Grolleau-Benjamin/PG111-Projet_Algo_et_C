/**
 * @file lz.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Lempel-Ziv compression algorithm.
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/common.h"

#define MAX 256
#define TABLE_SIZE 4096

typedef struct TreeNode {
    char key[MAX];
    int value;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode* lz_create_node(const char *key, int value);
TreeNode* lz_insert_node(TreeNode *node, const char *key, int value);
TreeNode* lz_search_node(TreeNode *node, const char *key);
void lz_free_tree(TreeNode *node);
void lz_encoding(const char *input_filename, const char *output_filename);
void lz_decoding(const char *input_filename, const char *output_filename);
void lz_file(const char *input_filename, char mode);