/* -*- mode: c; tab-width: 4; indent-tabs-mode: nil; -*- */

/*
* This file is part of liblzg.
*
* Copyright (c) 2010 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*/

#include <lzg.h>

/*-- PRIVATE -----------------------------------------------------------------*/

/* Internal definitions */
#define LZG_HEADER_SIZE 16
#define LZG_METHOD_COPY 0
#define LZG_METHOD_LZG1 1

/* Endian and alignment independent reader for 32-bit integers */
#define _LZG_GetUINT32(in, offs) \
    ((((lzg_uint32_t)in[offs]) << 24) | \
     (((lzg_uint32_t)in[offs+1]) << 16) | \
     (((lzg_uint32_t)in[offs+2]) << 8) | \
     ((lzg_uint32_t)in[offs+3]))

/* Calculate the checksum */
static lzg_uint32_t _LZG_CalcChecksum(const unsigned char *data, lzg_uint32_t size)
{
    unsigned short a = 1, b = 0;
    unsigned char *end = (unsigned char *)data + size;
    while (data != end)
    {
        a += *data++;
        b += a;
    }
    return (((lzg_uint32_t)b) << 16) | a;
}

/* LUT for decoding the copy length parameter */
static const unsigned char _LZG_LENGTH_DECODE_LUT[32] = {
    2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
    18,19,20,21,22,23,24,25,26,27,28,29,35,48,72,128
};


/*-- PUBLIC ------------------------------------------------------------------*/

lzg_uint32_t LZG_DecodedSize(const unsigned char *in, lzg_uint32_t insize)
{
    /* Check header */
    if ((insize < 7) || (in[0] != 'L') || (in[1] != 'Z') || (in[2] != 'G'))
        return 0;

    /* Get output buffer size */
    return _LZG_GetUINT32(in, 3);
}

lzg_uint32_t LZG_Decode(const unsigned char *in, lzg_uint32_t insize,
    unsigned char *out, lzg_uint32_t outsize)
{
    unsigned char *src, *inEnd, *dst, *outEnd, *copy, symbol, b, b2;
    unsigned char m1, m2, m3, m4, method;
    lzg_uint32_t  i, length, offset, encodedSize, decodedSize, checksum;

    /* Check magic ID */
    if ((insize < LZG_HEADER_SIZE) || (in[0] != 'L') || (in[1] != 'Z') || (in[2] != 'G'))
        return 0;

    /* Get header data */
    decodedSize = _LZG_GetUINT32(in, 3);
    encodedSize = _LZG_GetUINT32(in, 7);
    checksum = _LZG_GetUINT32(in, 11);

    /* Check sizes */
    if ((outsize < decodedSize) || (encodedSize != (insize - LZG_HEADER_SIZE)))
        return 0;

    /* Check checksum */
    if (_LZG_CalcChecksum(&in[LZG_HEADER_SIZE], encodedSize) != checksum)
        return 0;

    /* Initialize the byte streams */
    src = (unsigned char *)in + LZG_HEADER_SIZE;;
    inEnd = ((unsigned char *)in) + insize;
    dst = out;
    outEnd = out + outsize;

    /* Check which method to use */
    method = in[15];
    if (method == LZG_METHOD_LZG1)
    {
        if (!((src + 4) <= inEnd)) return 0;
        m1 = *src++; m2 = *src++; m3 = *src++; m4 = *src++;

        /* Main decompression loop */
        while (src < inEnd)
        {
            symbol = *src++;

            if ((symbol != m1) && (symbol != m2) && (symbol != m3) && (symbol != m4))
            {
                /* Literal copy */
                if (!(dst < outEnd)) return 0;
                *dst++ = symbol;
            }
            else
            {
                /* Decode offset / length parameters */
                if (!(src < inEnd)) return 0;
                if ((b = *src++))
                {
                    if (symbol == m1)
                    {
                        /* Distant copy */
                        if (!((src + 2) <= inEnd)) return 0;
                        length = _LZG_LENGTH_DECODE_LUT[b & 0x1f];
                        b2 = *src++;
                        offset = (((unsigned int)(b & 0xe0)) << 11) |
                                  (((unsigned int)b2) << 8) |
                                  (*src++);
                        offset += 2056;
                    }
                    else if (symbol == m2)
                    {
                        /* Medium copy */
                        if (!(src < inEnd)) return 0;
                        length = _LZG_LENGTH_DECODE_LUT[b & 0x1f];
                        b2 = *src++;
                        offset = (((unsigned int)(b & 0xe0)) << 3) | b2;
                        offset += 8;
                    }
                    else if (symbol == m3)
                    {
                        /* Short copy */
                        length = (b >> 6) + 3;
                        offset = (b & 0x3f) + 8;
                    }
                    else
                    {
                        /* Near copy (including RLE) */
                        length = _LZG_LENGTH_DECODE_LUT[b & 0x1f];
                        offset = (b >> 5) + 1;
                    }

                    /* Copy the corresponding data from the history window */
                    copy = dst - offset;
                    if (!((copy >= out) && ((dst + length) <= outEnd))) return 0;
                    for (i = 0; i < length; ++i)
                        *dst++ = *copy++;
                }
                else
                {
                    /* Literal copy (single occurance of a marker symbol) */
                    if (!(dst < outEnd)) return 0;
                    *dst++ = symbol;
                }
            }
        }
    }
    else if (method == LZG_METHOD_COPY)
    {
        /* Plain copy */
        while ((src < inEnd) && (dst < outEnd))
            *dst++ = *src++;
    }

    /* All OK? */
    if ((unsigned int)(dst - out) != decodedSize)
        return 0;
    else
        return decodedSize;
}
