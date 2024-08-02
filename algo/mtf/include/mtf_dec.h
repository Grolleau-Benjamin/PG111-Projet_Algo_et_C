/**
 *  @file mtf_dec.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Move-To-Front decompression algorithm.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MTF_DEC_H
#define MTF_DEC_H

#include "mtf_common.h"

#define MTF_OUTPUT_FILE_DEC_SUFFIX "_MTFdec"

int mtf_dec_char(mtf_t *mtf, char c);
int mtf_dec_file(mtf_t *mtf, const char *filename);

#endif