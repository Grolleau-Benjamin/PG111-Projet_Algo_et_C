/**
 * @file huffman.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the huffman algorithm.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef HUFFMAN
#define HUFFMAN

void huffman_encode(char *input_file, char *output_file);
void huffman_decode(char *input_file, char *output_file);
int huffman_file(char *filename, const char mode);

#endif