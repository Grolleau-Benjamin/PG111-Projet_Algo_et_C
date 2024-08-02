/**
 * @file binio.c
 * @author bgrolleau001 llunet001
 * @brief Binary I/O functions implementation.
 * This file implements the binary I/O functions defined in binio.h.
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
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


#include "../include/binio.h"

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
BFILE *bopen(const char *path, char mode, int padding)
{
    DEBUG_PRINT("[DEBUG] (BINIO) bopen(path = %s, mode = %c, padding = %d)\n", path, mode, padding);

    BFILE *bstream = (BFILE *)malloc(sizeof(BFILE));
    if (bstream == NULL)
    {
        return NULL;
    }

    switch (mode)
    {
    case 'r':
    case 'w':
    {
        char modeStr[3] = {mode, 'b', '\0'};
        bstream->file = fopen(path, modeStr);
        break;
    }
    default:
        free(bstream);
        return NULL;
    }

    if (bstream->file == NULL)
    {
        free(bstream);
        return NULL;
    }

    bstream->buffer = 0;
    bstream->buffer_len = 0;
    bstream->bit_pos = 0;
    bstream->mode = mode;
    bstream->padding = padding;

    return bstream;
}

/**
 * @brief Closes the binary stream. The bclose() function closes
 * the stream and frees the binary descriptor. In writting mode, if the padding
 * is enable, it writes the padding pattern before closing. This function must be
 * called to end any reading or writing.
 * @param stream The descriptor of the stream.
 * @return 0 upon success, otherwise EOF.
 */
int bclose(BFILE *stream)
{
    DEBUG_PRINT("[DEBUG] (BINIO) bclose(%p)\n", stream);

    if (stream == NULL)
    {
        return EOF;
    }

    if (stream->mode == 'w')
    {
        if (stream->padding)
        {
            if (stream->buffer == 0)
            {
                DEBUG_PRINT("[DEBUG] (BINIO) \tBuffer is empty so added 0x80\n");
                fputc(0x80, stream->file);
            }
            else
            {
                DEBUG_PRINT("[DEBUG] (BINIO) \tPadding bit added at position %d\n", stream->bit_pos);
                DEBUG_PRINT("[DEBUG] (BINIO) \tBuffer before padding: 0x%X\n", stream->buffer);
                stream->buffer |= (1 << (7 - stream->bit_pos));
                DEBUG_PRINT("[DEBUG] (BINIO) \tBuffer after padding: 0x%X\n", stream->buffer);
                fputc(stream->buffer, stream->file);
            }
        }
        else
        {
            if (stream->bit_pos > 0)
            {
                fputc(stream->buffer, stream->file);
            }
        }
    }

    int result = fclose(stream->file);
    free(stream);

    return result;
}

/**
 * @brief Reads bits in the stream. The bread() function reads at most nbits bits
 * from the stream pointed by bstream, and stores them at the location given by ptr.
 * The first bit read in the stream is stored in the least significant bit in the
 * location given by ptr. If the padding is enable, the last bit read in
 * the stream is the last bit before the padding pattern.
 * @param ptr The location where the read bits must be stored.
 *            This location must correspond to at least nbits /8 allocated bytes.
 * @param nbits The maximal number of read bits.
 * @param bstream The descriptor of the stream.
 * @return the number of bits read of the stream
 */
unsigned int bread(void *ptr, unsigned int nbits, BFILE *bstream)
{
    if (bstream == NULL || bstream->file == NULL)
    {
        return 0;
    }

    unsigned char *bytes = (unsigned char *)ptr;
    unsigned int bitsRead = 0;
    unsigned int byteIndex = 0;
    unsigned int bitPos = 0;

    unsigned int nbytes = (nbits + 7) / 8;
    for (unsigned int i = 0; i < nbytes; i++)
    {
        bytes[i] = 0;
    }

    for (unsigned int i = 0; i < nbits; i++)
    {
        int bit = bgetbit(bstream);
        if (bit == EOF)
        {
            break;
        }

        if (bit)
        {
            bytes[byteIndex] |= (1 << bitPos);
        }

        bitPos++;
        if (bitPos == 8)
        {
            bitPos = 0;
            byteIndex++;
        }
        bitsRead++;
    }

    return bitsRead;
}

/**
 * @brief Writes bits in the stream. The bwrite function writes nbits bits
 * to the stream pointed to by bstream, obtaining them from the location
 * given by ptr. The least significant bit in the location given by ptr
 * is the first bit written in the stream.
 * @param ptr The location where the read bits are read. This location
 *            must correspond to at least nbits /8 allocated bytes
 * @param nbits The maximal number of read bits.
 * @param bstream The descriptor of the stream.
 * @return the number of bits upon success.
 */
unsigned int bwrite(void *ptr, unsigned int nbits, BFILE *bstream)
{
    DEBUG_PRINT("[DEBUG] (BINIO) bwrite(%p, %d, %p)\n", ptr, nbits, bstream);
    if (ptr == NULL || bstream == NULL || bstream->file == NULL)
    {
        return 0;
    }

    unsigned char *bytes = (unsigned char *)ptr;
    unsigned int bitsWritten = 0;

    unsigned int nbytes = (nbits + 7) / 8;

    for (unsigned int i = 0; i < nbytes; i++)
    {
        for (int j = 0; j <= 7; j++)
        {
            if (bitsWritten < nbits)
            {
                bputbit((bytes[i] >> j) & 1, bstream);
                bitsWritten++;
            }
        }
    }

    return bitsWritten;
}

/**
 * @brief Reads one bit in the stream.
 * @param bstream The descriptor of the stream.
 * @return The value of the bit or EOF if there is no available bit in the stream.
 */
int bgetbit(BFILE *bstream)
{
    DEBUG_PRINT("[DEBUG] (BINIO) bgetbit(%p)\n", bstream);

    if (bstream == NULL || bstream->file == NULL)
    {
        return EOF;
    }

    if (bstream->bit_pos == 0)
    {
        if (feof(bstream->file))
        {
            return EOF;
        }
        int buffer = fgetc(bstream->file);
        if (buffer == EOF)
        {
            return EOF;
        }

        bstream->buffer = buffer;
        bstream->buffer_len = 8;
        bstream->bit_pos = 8;

        if (bstream->padding)
        {
            long long pos = ftell(bstream->file);
            fseek(bstream->file, 0, SEEK_END);
            long long end = ftell(bstream->file);
            fseek(bstream->file, pos, SEEK_SET);

            if (pos == end)
            {
                if (bstream->buffer == 0x80)
                {
                    return EOF;
                }
                else
                {
                    int paddingBitPos = 0;
                    while (paddingBitPos >= 0 && !((bstream->buffer >> paddingBitPos) & 1))
                    {
                        paddingBitPos++;
                    }

                    bstream->buffer_len -= paddingBitPos + 1;
                    DEBUG_PRINT("[DEBUG] (BINIO) Padding bit found at position %d\n", paddingBitPos);
                    DEBUG_PRINT("[DEBUG] (BINIO) Padding bits removed, new buffer_len = %d\n", bstream->buffer_len);
                    if (bstream->buffer_len == 0)
                    {
                        return EOF;
                    }
                }
            }
        }
    }

    bstream->bit_pos--;
    if (bstream->bit_pos == (7 - bstream->buffer_len))
    {
        return EOF;
    }
    int bit = (bstream->buffer >> bstream->bit_pos) & 1;
    return bit;
}

/**
 * @brief Writes one bit in the stream.
 * @param b The weakest bit of b is written in the stream
 * @param bstream The descriptor of the stream.
 */
void bputbit(uchar b, BFILE *bstream)
{
    DEBUG_PRINT("[DEBUG] (BINIO) bputbit(%d, %p)\n", b, bstream);

    if (bstream == NULL || bstream->file == NULL)
    {
        return;
    }

    if (bstream->bit_pos == 0)
    {
        bstream->bit_pos = 8;
        if (bstream->buffer_len == 8)
        {
            fputc(bstream->buffer, bstream->file);
            bstream->buffer = 0;
            bstream->buffer_len = 0;
        }
    }

    bstream->bit_pos--;
    bstream->buffer |= ((b & 1) << bstream->bit_pos);
    bstream->buffer_len++;
    DEBUG_PRINT("[DEBUG] (BINIO) \t bstream->buffer  = %d (0x%X)\n", bstream->buffer, bstream->buffer);
}