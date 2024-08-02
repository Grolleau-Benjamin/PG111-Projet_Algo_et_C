/**
 * @file mtf.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Move-To-Front algorithm.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MTF_H
#define MTF_H
#include "mtf_enc.h"
#include "mtf_dec.h"

int mtf_file(const char *filename, const char mode);
#endif