/**
 * @file huffman_dec.c
 * @author bgrolleau001 llunet001
 * @brief Implementation of Huffman decoding.
 * This file implements the Huffman decoding functions defined in huffman_dec.h.
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

#include "huffman_dec.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Reads a specified number of bits from a BFILE.
 *
 * @param bfile Pointer to the BFILE.
 * @param num_bits Number of bits to read.
 * @return The value read.
 */
unsigned char read_bits(BFILE *bfile, int num_bits)
{
    unsigned char value = 0;
    for (int i = 0; i < num_bits; i++)
    {
        int bit = bgetbit(bfile);
        if (bit == EOF)
        {
            fprintf(stderr, "Error: Unexpected end of file while reading bits\n");
            break;
        }
        value = (value << 1) | bit;
    }
    return value;
}

/**
 * @brief Creates a new leaf node.
 *
 * @param c Character value of the leaf.
 * @return Pointer to the new leaf node.
 */
struct leaf_dec *create_leaf(char c)
{
    struct leaf_dec *new_leaf = (struct leaf_dec *)malloc(sizeof(struct leaf_dec));
    if (!new_leaf)
    {
        perror("Error allocating memory for leaf");
        exit(EXIT_FAILURE);
    }
    new_leaf->c = c;
    return new_leaf;
}

/**
 * @brief Creates a new internal node.
 *
 * @param left Pointer to the left child.
 * @param right Pointer to the right child.
 * @return Pointer to the new internal node.
 */
struct node_dec *create_node(struct node_dec *left, struct node_dec *right)
{
    struct node_dec *new_node = (struct node_dec *)malloc(sizeof(struct node_dec));
    if (!new_node)
    {
        perror("Error allocating memory for node");
        exit(EXIT_FAILURE);
    }
    new_node->l = left;
    new_node->r = right;
    new_node->leaf = NULL;
    return new_node;
}

/**
 * @brief Constructs the Huffman tree from the encoded string.
 *
 * @param bfile Pointer to the BFILE containing the encoded string.
 * @return Pointer to the root of the Huffman tree.
 */
struct node_dec *construct_tree(BFILE *bfile)
{
    int bit = bgetbit(bfile);
    if (bit == EOF)
    {
        return NULL;
    }

    if (bit == 1)
    {
        char c = read_bits(bfile, 8);
        struct leaf_dec *leaf = create_leaf(c);
        struct node_dec *leaf_node = (struct node_dec *)malloc(sizeof(struct node_dec));
        if (!leaf_node)
        {
            perror("Error allocating memory for leaf node");
            exit(EXIT_FAILURE);
        }
        leaf_node->l = leaf_node->r = NULL;
        leaf_node->leaf = leaf;
        return leaf_node;
    }
    else if (bit == 0)
    {
        struct node_dec *left = construct_tree(bfile);
        struct node_dec *right = construct_tree(bfile);
        return create_node(left, right);
    }
    return NULL;
}

/**
 * @brief Prints the Huffman tree for debugging purposes.
 *
 * @param root Pointer to the root of the Huffman tree.
 * @param depth Current depth in the tree.
 */
void print_tree_dec(struct node_dec *root, int depth)
{
    if (root == NULL)
        return;
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    if (root->leaf != NULL)
    {
        printf("Leaf: %c\n", root->leaf->c);
    }
    else
    {
        printf("Node\n");
        print_tree_dec(root->l, depth + 1);
        print_tree_dec(root->r, depth + 1);
    }
}

/**
 * @brief Frees the memory allocated for the Huffman tree.
 *
 * @param root Pointer to the root of the Huffman tree.
 */
void free_tree_dec(struct node_dec *root)
{
    if (root == NULL)
        return;
    free_tree_dec(root->l);
    free_tree_dec(root->r);
    if (root->leaf != NULL)
    {
        free(root->leaf);
    }
    free(root);
}

/**
 * @brief Decodes the message using the Huffman tree and writes it to the output file.
 *
 * @param root Pointer to the root of the Huffman tree.
 * @param bfile Pointer to the BFILE containing the encoded message.
 * @param output Pointer to the output file.
 */
void decode_message(struct node_dec *root, BFILE *bfile, FILE *output)
{
    // Read the entire bitstream into a buffer
    int bit;
    int buffer_size = 1024;
    unsigned char *buffer = (unsigned char *)calloc(buffer_size, sizeof(unsigned char));
    if (!buffer)
    {
        perror("Error allocating memory for buffer");
        exit(EXIT_FAILURE);
    }
    int bit_count = 0;

    while ((bit = bgetbit(bfile)) != EOF)
    {
        if (bit_count >= buffer_size * 8)
        {
            buffer_size *= 2;
            buffer = (unsigned char *)realloc(buffer, buffer_size);
            if (!buffer)
            {
                perror("Error reallocating memory for buffer");
                exit(EXIT_FAILURE);
            }
        }
        if (bit)
        {
            buffer[bit_count / 8] |= (1 << (7 - (bit_count % 8)));
        }
        else
        {
            buffer[bit_count / 8] &= ~(1 << (7 - (bit_count % 8)));
        }
        bit_count++;
    }

    // Find the position of the last '1' bit (which starts the padding)
    int last_one_position = -1;
    for (int i = bit_count; i >= 0; i--)
    {
        if ((buffer[i / 8] & (1 << (7 - (i % 8)))) != 0)
        {
            last_one_position = i;
            break;
        }
    }

    // printf("Buffer content (in bits):\n");
    // for (int i = 0; i < bit_count; i++)
    // {
    //     int byte_index = i / 8;
    //     int bit_index = 7 - (i % 8);
    //     int bit_value = (buffer[byte_index] >> bit_index) & 1;
    //     printf("%d", bit_value);
    //     if ((i + 1) % 8 == 0)
    //     {
    //         printf(" "); // Add space for better readability
    //     }
    // }
    // printf("\n");

    if (last_one_position == -1)
    {
        fprintf(stderr, "Invalid padding in the bitstream\n");
        free(buffer);
        return;
    }

    // Decode the buffer up to the last '1' bit
    struct node_dec *current = root;
    for (int i = 0; i <= last_one_position; i++)
    {
        bit = (buffer[i / 8] & (1 << (7 - (i % 8)))) != 0;
        if (bit == 0)
        {
            current = current->l;
        }
        else if (bit == 1)
        {
            current = current->r;
        }

        if (current->leaf != NULL)
        {
            fputc(current->leaf->c, output);
            current = root; // Go back to the root for the next character
        }
    }

    free(buffer);
}

/**
 * @brief Decodes the encoded file.
 *
 * @param input_file Path to the input file containing the encoded message.
 * @param output_file Path to the output file where the decoded message will be written.
 */
void huff_decode(const char *input_file, const char *output_file)
{
    BFILE *bfile = bopen(input_file, 'r', 0);
    if (!bfile)
    {
        perror("Error opening input file");
        return;
    }

    struct node_dec *root = construct_tree(bfile);
    if (root == NULL)
    {
        fprintf(stderr, "Failed to construct Huffman tree\n");
        bclose(bfile);
        return;
    }

    FILE *output = fopen(output_file, "wb");
    if (!output)
    {
        perror("Error opening output file");
        free_tree_dec(root);
        bclose(bfile);
        return;
    }

    decode_message(root, bfile, output);

    // print_tree_dec(root, 0);
    free_tree_dec(root);
    bclose(bfile);
    fclose(output);
}
