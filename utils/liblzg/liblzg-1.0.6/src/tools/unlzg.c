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

#include <stdio.h>
#include <stdlib.h>
#include <lzg.h>

int main(int argc, char **argv)
{
    FILE *inFile, *outFile;
    size_t fileSize;
    unsigned char *encBuf;
    lzg_uint32_t encSize = 0;
    unsigned char *decBuf;
    lzg_uint32_t decSize;
    int useStdout = 0;

    // Check arguments
    if ((argc < 2) || (argc > 3))
    {
        fprintf(stderr, "Usage: %s infile [outfile]\n", argv[0]);
        fprintf(stderr, "If no output file is given, stdout is used for output.\n");
        return 0;
    }

    // Use stdout?
    if (argc < 3)
        useStdout = 1;

    // Read input file
    encBuf = (unsigned char*) 0;
    inFile = fopen(argv[1], "rb");
    if (inFile)
    {
        fseek(inFile, 0, SEEK_END);
        fileSize = ftell(inFile);
        fseek(inFile, 0, SEEK_SET);
        if (fileSize > 0)
        {
            encSize = (lzg_uint32_t) fileSize;
            encBuf = (unsigned char*) malloc(encSize);
            if (encBuf)
            {
                if (fread(encBuf, 1, encSize, inFile) != encSize)
                {
                    fprintf(stderr, "Error reading \"%s\".\n", argv[1]);
                    free(encBuf);
                    encBuf = (unsigned char*) 0;
                }
            }
            else
                fprintf(stderr, "Out of memory.\n");
        }
        else
            fprintf(stderr, "Input file is empty.\n");

        fclose(inFile);
    }
    else
        fprintf(stderr, "Unable to open file \"%s\".\n", argv[1]);

    if (!encBuf)
        return 0;

    // Determine size of decompressed data
    decSize = LZG_DecodedSize(encBuf, encSize);
    if (decSize)
    {
        // Allocate memory for the decompressed data
        decBuf = (unsigned char*) malloc(decSize);
        if (decBuf)
        {
            // Decompress
            decSize = LZG_Decode(encBuf, encSize, decBuf, decSize);
            if (decSize)
            {
                // Uncompressed data is now in decBuf, write it...
                if (!useStdout)
                {
                    outFile = fopen(argv[2], "wb");
                    if (!outFile)
                        fprintf(stderr, "Unable to open file \"%s\".\n", argv[2]);
                }
                else
                    outFile = stdout;

                if (outFile)
                {
                    // Write data
                    if (fwrite(decBuf, 1, decSize, outFile) != decSize)
                        fprintf(stderr, "Error writing to output file.\n");

                    // Close file
                    if (!useStdout)
                        fclose(outFile);
                }
            }
            else
                fprintf(stderr, "Decompression failed (bad data)!\n");

            // Free memory when we're done with the decompressed data
            free(decBuf);
        }
        else
            fprintf(stderr, "Out of memory!\n");
    }
    else
        fprintf(stderr, "Bad input data!\n");

    // Free memory
    free(encBuf);

    return 0;
}

