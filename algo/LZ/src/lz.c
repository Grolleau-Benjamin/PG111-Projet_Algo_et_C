/**
 * @file lz.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of the Lempel-Ziv compression algorithm.
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

/**
 * @brief Creates a new tree node.
 *
 * @param key The key of the node.
 * @param value The value associated with the key.
 * @return A pointer to the new node.
 */
TreeNode* lz_create_node(const char *key, int value) {
    TreeNode *new_node = (TreeNode *)malloc(sizeof(TreeNode));
    strcpy(new_node->key, key);
    new_node->value = value;
    new_node->left = new_node->right = NULL;
    return new_node;
}

/**
 * @brief Inserts a new node into the tree.
 *
 * @param node The root node of the tree.
 * @param key The key to insert.
 * @param value The value associated with the key.
 * @return The root node of the modified tree.
 */
TreeNode* lz_insert_node(TreeNode *node, const char *key, int value) {
    if (node == NULL) return lz_create_node(key, value);

    int cmp = strcmp(key, node->key);
    if (cmp < 0)
        node->left = lz_insert_node(node->left, key, value);
    else if (cmp > 0)
        node->right = lz_insert_node(node->right, key, value);

    return node;
}

/**
 * @brief Searches for a node in the tree.
 *
 * @param node The root node of the tree.
 * @param key The key to search for.
 * @return A pointer to the found node, or NULL if the node does not exist.
 */
TreeNode* lz_search_node(TreeNode *node, const char *key) {
    if (node == NULL || strcmp(key, node->key) == 0)
        return node;

    int cmp = strcmp(key, node->key);
    if (cmp < 0)
        return lz_search_node(node->left, key);
    else
        return lz_search_node(node->right, key);
}

/**
 * @brief Frees the memory allocated for the tree.
 *
 * @param node The root node of the tree to free.
 */
void lz_free_tree(TreeNode *node) {
    if (node == NULL) return;
    lz_free_tree(node->left);
    lz_free_tree(node->right);
    free(node);
}

/**
 * @brief Encodes a file using the LZ78 algorithm.
 *
 * @param input_filename The name of the input file.
 * @param output_filename The name of the output file.
 */
void lz_encoding(const char *input_filename, const char *output_filename) {
    DEBUG_PRINT("Encoding\n");
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    if (!input_file || !output_file) {
        perror("Error opening file");
        if (input_file) fclose(input_file);
        if (output_file) fclose(output_file);
        return;
    }

    TreeNode *root = NULL;
    for (int i = 0; i <= 255; i++) {
        char ch[2] = { (char)i, '\0' };
        root = lz_insert_node(root, ch, i);
    }

    char p[MAX] = "", c[2] = "";
    int code = 256;
    DEBUG_PRINT("String\tOutput_Code\tAddition\n");

    int ch;
    while ((ch = fgetc(input_file)) != EOF) {
        c[0] = (char)ch;
        c[1] = '\0';

        char pc[MAX];
        strcpy(pc, p);
        strcat(pc, c);

        if (lz_search_node(root, pc) != NULL) {
            strcpy(p, pc);
        } else {
            TreeNode *node = lz_search_node(root, p);
            if (node) {
                DEBUG_PRINT("%s\t%d\t\t%s\t%d\n", p, node->value, pc, code);
                fprintf(output_file, "%d ", node->value);
                root = lz_insert_node(root, pc, code);
                code++;
            } else {
                fprintf(stderr, "Error: node with key '%s' not found in the tree.\n", p);
            }
            strcpy(p, c);
        }
    }

    TreeNode *node = lz_search_node(root, p);
    if (node) {
        DEBUG_PRINT("%s\t%d\n", p, node->value);
        fprintf(output_file, "%d\n", node->value);
    }

    fclose(input_file);
    fclose(output_file);
    lz_free_tree(root);
}

/**
 * @brief Decodes a file using the LZ78 algorithm.
 *
 * @param input_filename The name of the input file.
 * @param output_filename The name of the output file.
 */
void lz_decoding(const char *input_filename, const char *output_filename) {
    DEBUG_PRINT("\nDecoding\n");
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    if (!input_file || !output_file) {
        perror("Error opening file");
        if (input_file) fclose(input_file);
        if (output_file) fclose(output_file);
        return;
    }

    char *table[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = (char *)malloc(MAX * sizeof(char));
        if (i < 256) {
            table[i][0] = (char)i;
            table[i][1] = '\0';
        } else {
            table[i][0] = '\0';
        }
    }

    int old, n;
    if (fscanf(input_file, "%d", &old) != 1) {
        fprintf(stderr, "Error reading the first integer from file.\n");
        fclose(input_file);
        fclose(output_file);
        return;
    }
    DEBUG_PRINT("First code: %d\n", old);

    fprintf(output_file, "%s", table[old]);
    DEBUG_PRINT("Decoded first string: %s\n", table[old]);

    int count = 256;
    char s[MAX], c[2];
    strcpy(s, table[old]);
    c[0] = s[0];
    c[1] = '\0';

    while (fscanf(input_file, "%d", &n) != EOF) {
        DEBUG_PRINT("Read code: %d\n", n);
        if (n >= count) {
            DEBUG_PRINT("Code %d not found in table. Handling special case.\n", n);
            strcpy(s, table[old]);
            strcat(s, c);
        } else {
            strcpy(s, table[n]);
        }
        DEBUG_PRINT("Decoded string: %s\n", s);
        fprintf(output_file, "%s", s);
        c[0] = s[0];
        c[1] = '\0';
        strcpy(table[count], table[old]);
        strcat(table[count], c);
        DEBUG_PRINT("Inserting new entry: %s with code %d\n", table[count], count);
        count++;
        old = n;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        free(table[i]);
    }

    fclose(input_file);
    fclose(output_file);
}

/**
 * @brief Encodes or decodes a file using the LZ78 algorithm.
 *
 * @param input_filename The name of the input file.
 * @param mode The mode of operation ('e' for encoding, 'd' for decoding).
 */
void lz_file(const char *input_filename, char mode) {
    char output_filename[256];
    if (mode == 'e') {
        printf("(LEMPEL-ZIV) Encoding file: %s\n", input_filename);
        sprintf(output_filename, "%s_LZenc", input_filename);
        lz_encoding(input_filename, output_filename);
    } else if (mode == 'd') {
        printf("(LEMPEL-ZIV) Decoding file: %s\n", input_filename);
        sprintf(output_filename, "%s_LZdec", input_filename);
        lz_decoding(input_filename, output_filename);
    } else {
        fprintf(stderr, "Error: invalid mode '%c'.\n", mode);
    }
}
