/**
 * @file common.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the common functions for all algorihms.
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
*/

#ifndef COMMON_H
#define COMMON_H
    #include <stdio.h>

    #ifdef DEBUG
        #include <assert.h>
        #define DEBUG_PRINT(...) printf(__VA_ARGS__)
    #else
        #define DEBUG_PRINT(...) (void)0
    #endif

    #define NULL_ERROR -1
    #define FILE_ERROR -2
    #define MEMORY_ERROR -3
    #define VALUE_ERROR -4

#endif