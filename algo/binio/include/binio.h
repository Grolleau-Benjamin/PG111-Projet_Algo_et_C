/**
 * @file binio.h
 * @author bgrolleau001 llunet001
 * @brief Header file for the binary stream I/O operations.
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BINIO_H
#define BINIO_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "../../common/common.h"

    /**
     * @brief A type definition for an unsigned char.
     * This is used to represent a byte in the binary stream.
     * uchar is an alias for unsigned char.
    */
    typedef unsigned char uchar;

    /**
     * @brief A type definition for a binary stream descriptor.
     * This is used to represent a binary stream.
    */
    struct bfile {
        FILE *file;
        uchar buffer;
        unsigned short int buffer_len;
        unsigned short int bit_pos;
        char mode;
        unsigned short int padding;
    };
    /**
     * @brief A type definition for a binary stream descriptor.
     * This is used to represent a binary stream.
     * BFILE is an alias for struct bfile.
    */
    typedef struct bfile BFILE;

    /**
     * @brief Opens a binary stream. The bopen() function opens
     * the file whose name is the string pointed by path and
     * associates a binary stream with it.
     * @param path The relative or absolute path to the file.
     * @param mode Accepts only two values: 
     *      - 'r' for reading; the stream is positioned at the beginning of the file.
     *      - 'w' for writing; the stream is positioned at the beginning of the file; 
     *        if the file does not exist, it is created, otherwise, it is overwritten.
     * @param padding Boolean value which is true if and only if a padding at the end of the file.
     * @return A pointer to a binary descriptor if successful, otherwise, NULL.
    */
    BFILE *bopen(const char *path, char mode, int padding);

    /**
     * @brief Closes the binary stream. The bclose() function closes
     * the stream and frees the binary descriptor. In writting mode, if the padding
     * is enable, it writes the padding pattern before closing. This function must be
     * called to end any reading or writing.
     * @param stream The descriptor of the stream.
     * @return 0 upon success, otherwise EOF.
    */
    int bclose(BFILE *stream);

    unsigned int bread(void* ptr, unsigned int nbits, BFILE *bstream);
    unsigned int bwrite(void* ptr, unsigned int nbits, BFILE *bstream);

    int bgetbit(BFILE *bstream);
    void bputbit(uchar b, BFILE *bstream);

#endif