/**
 * @file huffman_enc.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Huffman encoding algorithm.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef HUFFMAN_ENC
#define HUFFMAN_ENC

#define HUFF_OUTPUT_FILE_ENC_SUFFIX "_HUFFenc"
struct frequency_tab
{
    int arr[256];
    int size;
};

struct node
{
    struct node *l;
    struct node *r;
    struct leaf *leaf;
    int value;
};

struct leaf
{
    char c;
    int value;
};

void init_frequency_tab(struct frequency_tab *ft);
void update_frequency_tab(struct frequency_tab *ft, const char *file);
void print_frequency_tab(struct frequency_tab *ft);
struct node *build_tree(struct frequency_tab *ft, struct leaf *leaves, struct node *nodes, int leaf_count);
void generate_codes(struct node *tree, char *code, int depth, char *codes[]);
void encode_file(const char *input_file, const char *output_file, struct node *tree, char *codes[]);
void print_tree(struct node *n);
void free_tree(struct node *tree);
void huff_encode(char *input_file, char *output_file);
#endif
