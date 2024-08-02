/**
 * @file mtf_common.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the Move-To-Front common functions.
 * @version 0.1
 * @date 2024-05-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MTF_COMMON_H
#define MTF_COMMON_H

#include "../../common/common.h"

/**
 * @brief A type definition for the Move-to-Front (MTF) structure.
 * This is used to represent the MTF structure.
 */
struct mtf_struct
{
    char arr[256];
    int size;
};
/**
 * @brief A type definition for the Move-to-Front (MTF) structure.
 * This is used to represent the MTF structure.
 * mtf_t is an alias for struct mtf_struct.
 */
typedef struct mtf_struct mtf_t;

int mtf_init(mtf_t *mtf, unsigned int size);

#endif