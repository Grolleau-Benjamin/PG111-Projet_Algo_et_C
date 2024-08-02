/**
 * @file huffman_dec.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Huffman decoding algorithm.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef HUFFMAN_DEC
#define HUFFMAN_DEC

#include "../../binio/include/binio.h"

#include <stdio.h>

#define HUFF_OUTPUT_FILE_DEC_SUFFIX "_HUFFdec"
struct leaf_dec
{
    char c;
};

struct node_dec
{
    struct node_dec *l;
    struct node_dec *r;
    struct leaf_dec *leaf;
};

// Helper function to read a specified number of bits from a BFILE
unsigned char read_bits(BFILE *bfile, int num_bits);

// Helper function to create a new leaf
struct leaf_dec *create_leaf(char c);

// Helper function to create a new node
struct node_dec *create_node(struct node_dec *left, struct node_dec *right);

// Function to construct the tree from the encoded string
struct node_dec *construct_tree(BFILE *bfile);

// Function to print the tree for debugging purposes
void print_tree_dec(struct node_dec *root, int depth);

// Function to free the memory allocated for the tree
void free_tree_dec(struct node_dec *root);

// Function to decode the message using the Huffman tree and write it to the output file
void decode_message(struct node_dec *root, BFILE *bfile, FILE *output);

// Function to decode the input file using the Huffman tree and write the decoded output to a file
void huff_decode(const char *input_file, const char *output_file);

#endif // HUFFMAN_DEC
