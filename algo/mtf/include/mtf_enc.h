/**
 * @file mtf_enc.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Move-To-Front encoding algorithm.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MTF_ENC_H
#define MTF_ENC_H

#include "mtf_common.h"

#define MTF_OUTPUT_FILE_ENC_SUFFIX "_MTFenc"

int mtf_enc_char(mtf_t *mtf, char c);
int mtf_enc_file(mtf_t *mtf, const char *filename);

#endif