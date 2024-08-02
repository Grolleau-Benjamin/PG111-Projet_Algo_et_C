/**
 * @file huffman_enc.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of Huffman encoding.
 * This file implements the Huffman encoding functions defined in huffman_enc.h.
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

#include "huffman_enc.h"
#include "../../binio/include/binio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initializes the frequency table with zeros.
 *
 * @param ft Pointer to the frequency table.
 */
void init_frequency_tab(struct frequency_tab *ft)
{
    for (int i = 0; i < 256; i++)
    {
        ft->arr[i] = 0;
    }
    ft->size = 256;
}

/**
 * @brief Updates the frequency table based on the contents of a file.
 *
 * @param ft Pointer to the frequency table.
 * @param file Path to the file.
 */
void update_frequency_tab(struct frequency_tab *ft, const char *file)
{
    FILE *f = fopen(file, "rb");

    if (f == NULL)
    {
        perror("Error opening file");
        return;
    }

    int chr;
    while ((chr = fgetc(f)) != EOF)
    {
        ft->arr[(unsigned char)chr]++; // Ensure correct indexing
    }

    fclose(f);
}

/**
 * @brief Prints the contents of the frequency table.
 *
 * @param ft Pointer to the frequency table.
 */
void print_frequency_tab(struct frequency_tab *ft)
{
    for (int i = 0; i < ft->size; i++)
    {
        if (ft->arr[i] > 0)
        {
            printf("%c -> %d\n", i, ft->arr[i]);
        }
    }
}

/**
 * @brief Sorts an array of nodes based on their values.
 *
 * @param nodes Array of nodes.
 * @param count Number of nodes in the array.
 */
void sort_nodes(struct node nodes[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (nodes[j].value > nodes[j + 1].value)
            {
                struct node temp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Prints the Huffman tree.
 *
 * @param n Pointer to the root of the tree.
 */
void print_tree(struct node *n)
{
    if (n == NULL)
        return;
    if (n->leaf != NULL)
    {
        printf("Leaf: %c, Frequency: %d\n", n->leaf->c, n->leaf->value);
    }
    else
    {
        printf("Node: Frequency: %d\n", n->value);
        print_tree(n->l);
        print_tree(n->r);
    }
}

/**
 * @brief Builds the Huffman tree from the frequency table.
 *
 * @param ft Pointer to the frequency table.
 * @return Pointer to the root of the Huffman tree.
 */
struct node *build_tree(struct frequency_tab *ft, struct leaf *leaves, struct node *nodes, int leaf_count)
{

    int index = 0;
    for (int i = 0; i < ft->size; i++)
    {
        if (ft->arr[i] > 0)
        {
            leaves[index].c = i;
            leaves[index].value = ft->arr[i];
            nodes[index].leaf = &leaves[index];
            nodes[index].l = nodes[index].r = NULL;
            nodes[index].value = ft->arr[i];
            index++;
        }
    }

    sort_nodes(nodes, leaf_count);

    int node_count = leaf_count;
    while (node_count > 1)
    {
        struct node *left = (struct node *)malloc(sizeof(struct node));
        struct node *right = (struct node *)malloc(sizeof(struct node));
        *left = nodes[0];
        *right = nodes[1];

        struct node new_node = {left, right, NULL, left->value + right->value};
        for (int i = 0; i < node_count - 2; i++)
        {
            nodes[i] = nodes[i + 2];
        }
        nodes[node_count - 2] = new_node;
        node_count--;

        sort_nodes(nodes, node_count);
    }

    struct node *tree_root = (struct node *)malloc(sizeof(struct node));
    *tree_root = nodes[0];

    return tree_root;
}

/**
 * @brief Generates Huffman codes from the Huffman tree.
 *
 * @param tree Pointer to the root of the Huffman tree.
 * @param code Pointer to the current code.
 * @param depth Current depth in the tree.
 * @param codes Array to store the generated codes.
 */
void generate_codes(struct node *tree, char *code, int depth, char *codes[])
{
    if (tree->leaf != NULL)
    {
        code[depth] = '\0';
        codes[(unsigned char)tree->leaf->c] = strdup(code);
        return;
    }

    if (tree->l != NULL)
    {
        code[depth] = '0';
        generate_codes(tree->l, code, depth + 1, codes);
    }

    if (tree->r != NULL)
    {
        code[depth] = '1';
        generate_codes(tree->r, code, depth + 1, codes);
    }
}

/**
 * @brief Converts a character to its binary representation and writes it to a binary file.
 *
 * @param c Character to convert.
 * @param output Pointer to the binary file.
 */
void char_to_binary(char c, BFILE *output)
{
    for (int i = 7; i >= 0; i--)
    {
        unsigned char bit = (c & (1 << i)) ? 1 : 0;
        bputbit(bit, output);
    }
}

/**
 * @brief Writes the Huffman tree to a binary file.
 *
 * @param tree Pointer to the root of the Huffman tree.
 * @param output Pointer to the binary file.
 */
void write_dictionary(struct node *tree, BFILE *output)
{
    if (tree->leaf != NULL)
    {
        bputbit(1, output);
        char_to_binary(tree->leaf->c, output);
    }
    else
    {
        bputbit(0, output);
        write_dictionary(tree->l, output);
        write_dictionary(tree->r, output);
    }
}

/**
 * @brief Encodes a file using Huffman coding.
 *
 * @param input_file Path to the input file.
 * @param output_file Path to the output file.
 * @param tree Pointer to the root of the Huffman tree.
 * @param codes Array of Huffman codes.
 */
void encode_file(const char *input_file, const char *output_file, struct node *tree, char *codes[])
{
    FILE *input = fopen(input_file, "rb");
    BFILE *output = bopen(output_file, 'w', 0); // Disable automatic padding

    if (input == NULL || output == NULL)
    {
        perror("Error opening files");
        return;
    }

    // Write dictionary to output file
    write_dictionary(tree, output);

    // Encode file content
    int chr;
    int bits_written = 0; // Track the number of bits written
    while ((chr = fgetc(input)) != EOF)
    {
        char *code = codes[chr];
        for (int i = 0; code[i] != '\0'; i++)
        {
            unsigned char bit = code[i] - '0';
            bputbit(bit, output);
            bits_written++;
        }
    }

    // Calculate the number of padding bits needed
    int padding_bits = (8 - (bits_written % 8)) % 8;

    // Write the padding bits (1 followed by 0s)
    bputbit(1, output);
    for (int i = 0; i < padding_bits - 1; i++)
    {
        bputbit(0, output);
    }

    bclose(output);
    fclose(input);
}

/**
 * @brief Frees memory allocated for the Huffman tree.
 *
 * @param tree Pointer to the root of the Huffman tree.
 */
void free_tree(struct node *tree)
{
    if (tree == NULL)
        return;
    free_tree(tree->l);
    free_tree(tree->r);
    free(tree);
}

void huff_encode(char *input_file, char *output_file)
{
    struct frequency_tab ft;
    init_frequency_tab(&ft);
    update_frequency_tab(&ft, input_file);
    // print_frequency_tab(&ft);

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
    // print_tree(tree);

    char *codes[256] = {0};
    char code[256];
    generate_codes(tree, code, 0, codes);

    encode_file(input_file, output_file, tree, codes);

    // Free allocated memory
    free_tree(tree);
    for (int i = 0; i < 256; i++)
    {
        if (codes[i])
        {
            free(codes[i]);
        }
    }
    free(leaves);
    free(nodes);
}
