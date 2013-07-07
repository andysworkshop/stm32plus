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
#include <string.h>
#include <lzg.h>


void ShowProgress(int progress, void *data)
{
    FILE *f = (FILE *)data;
    fprintf(f, "Progress: %d%%   \r", progress);
    fflush(f);
}

void ShowUsage(char *prgName)
{
    fprintf(stderr, "Usage: %s [options] infile [outfile]\n", prgName);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, " -1  Use fastest compression\n");
    fprintf(stderr, " -9  Use best compression\n");
    fprintf(stderr, " -s  Do not use the fast method (saves memory)\n");
    fprintf(stderr, " -v  Be verbose\n");
    fprintf(stderr, " -V  Show LZG library version and exit\n");
    fprintf(stderr, "\nIf no output file is given, stdout is used for output.\n");
}

int main(int argc, char **argv)
{
    char *inName, *outName;
    FILE *inFile, *outFile;
    size_t fileSize;
    unsigned char *decBuf;
    lzg_uint32_t decSize = 0;
    unsigned char *encBuf;
    lzg_uint32_t maxEncSize, encSize;
    int arg, verbose;
    lzg_encoder_config_t config;

    // Default arguments
    inName = NULL;
    outName = NULL;
    LZG_InitEncoderConfig(&config);
    config.fast = LZG_TRUE;
    verbose = 0;

    // Get arguments
    for (arg = 1; arg < argc; ++arg)
    {
        if (strcmp("-1", argv[arg]) == 0)
            config.level = LZG_LEVEL_1;
        else if (strcmp("-2", argv[arg]) == 0)
            config.level = LZG_LEVEL_2;
        else if (strcmp("-3", argv[arg]) == 0)
            config.level = LZG_LEVEL_3;
        else if (strcmp("-4", argv[arg]) == 0)
            config.level = LZG_LEVEL_4;
        else if (strcmp("-5", argv[arg]) == 0)
            config.level = LZG_LEVEL_5;
        else if (strcmp("-6", argv[arg]) == 0)
            config.level = LZG_LEVEL_6;
        else if (strcmp("-7", argv[arg]) == 0)
            config.level = LZG_LEVEL_7;
        else if (strcmp("-8", argv[arg]) == 0)
            config.level = LZG_LEVEL_8;
        else if (strcmp("-9", argv[arg]) == 0)
            config.level = LZG_LEVEL_9;
        else if (strcmp("-s", argv[arg]) == 0)
            config.fast = LZG_FALSE;
        else if (strcmp("-v", argv[arg]) == 0)
            verbose = 1;
        else if (strcmp("-V", argv[arg]) == 0)
        {
            printf("LZG library version %s\n", LZG_VersionString());
            return 0;
        }
        else if (!inName)
            inName = argv[arg];
        else if (!outName)
            outName = argv[arg];
        else
        {
            ShowUsage(argv[0]);
            return 0;
        }
    }
    if (!inName)
    {
        ShowUsage(argv[0]);
        return 0;
    }

    // Read input file
    decBuf = (unsigned char*) 0;
    inFile = fopen(inName, "rb");
    if (inFile)
    {
        fseek(inFile, 0, SEEK_END);
        fileSize = (size_t) ftell(inFile);
        fseek(inFile, 0, SEEK_SET);
        if (fileSize > 0)
        {
            decSize = (lzg_uint32_t) fileSize;
            decBuf = (unsigned char*) malloc(decSize);
            if (decBuf)
            {
                if (fread(decBuf, 1, decSize, inFile) != decSize)
                {
                    fprintf(stderr, "Error reading \"%s\".\n", inName);
                    free(decBuf);
                    decBuf = (unsigned char*) 0;
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
        fprintf(stderr, "Unable to open file \"%s\".\n", inName);

    if (!decBuf)
        return 0;

    // Determine maximum size of compressed data
    maxEncSize = LZG_MaxEncodedSize(decSize);

    // Allocate memory for the compressed data
    encBuf = (unsigned char*) malloc(maxEncSize);
    if (encBuf)
    {
        // Compress
        if (verbose)
        {
            config.progressfun = ShowProgress;
            config.userdata = stderr;
        }
        encSize = LZG_Encode(decBuf, decSize, encBuf, maxEncSize, &config);
        if (encSize)
        {
            if (verbose)
            {
                fprintf(stderr, "Result: %d bytes (%d%% of the original)\n",
                                encSize, (100 * encSize) / decSize);
            }

            // Compressed data is now in encBuf, write it...
            if (outName)
            {
                outFile = fopen(outName, "wb");
                if (!outFile)
                    fprintf(stderr, "Unable to open file \"%s\".\n", outName);
            }
            else
                outFile = stdout;

            if (outFile)
            {
                // Write data
                if (fwrite(encBuf, 1, encSize, outFile) != encSize)
                    fprintf(stderr, "Error writing to output file.\n");

                // Close file
                if (outName)
                    fclose(outFile);
            }
        }
        else
            fprintf(stderr, "Compression failed!\n");

        // Free memory when we're done with the compressed data
        free(encBuf);
    }
    else
        fprintf(stderr, "Out of memory!\n");

    // Free memory
    free(decBuf);

    return 0;
}

