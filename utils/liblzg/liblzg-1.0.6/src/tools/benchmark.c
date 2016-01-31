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

#ifdef USE_ZLIB
# include <zlib.h>
#endif
#ifdef USE_BZ2
# include <bzlib.h>
#endif
#ifdef USE_LZO
# include <lzo/lzo1x.h>
#endif

#if defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
# define UNUSED(x) x
#endif


/*-- High resolution timer implementation -----------------------------------*/

#ifdef _WIN32
# include <windows.h>
#else
# include <sys/time.h>
#endif

#ifdef _WIN32
static __int64 g_timeStart;
static __int64 g_timeFreq;
#else
static long long g_timeStart;
#endif

void StartTimer(void)
{
#ifdef _WIN32
  if(QueryPerformanceFrequency((LARGE_INTEGER *)&g_timeFreq))
    QueryPerformanceCounter((LARGE_INTEGER *)&g_timeStart);
  else
    g_timeFreq = 0;
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  g_timeStart = (long long) tv.tv_sec * (long long) 1000000 + (long long) tv.tv_usec;
#endif
}

unsigned int StopTimer(void)
{
#ifdef _WIN32
  __int64 t;
  QueryPerformanceCounter((LARGE_INTEGER *)&t);
  return ((t - g_timeStart) * 1000000) / g_timeFreq;
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (long long) tv.tv_sec * (long long) 1000000 + (long long) tv.tv_usec - g_timeStart;
#endif
}

/*-- (end of high resolution timer implementation) --------------------------*/


/*-- Dynamic codec class ----------------------------------------------------*/

typedef unsigned int (*MAXENCODEDSIZEFUN)(unsigned int insize);
typedef unsigned int (*ENCODEFUN)(const unsigned char *decBuf, unsigned int decSize,
                                  unsigned char *encBuf, unsigned int maxEncSize,
                                  int level, int fast, LZGPROGRESSFUN progressfun,
                                  void *userdata);
typedef unsigned int (*DECODEFUN)(const unsigned char *encBuf, unsigned int encSize,
                                  unsigned char *decBuf, unsigned int decSize);

typedef struct _codec_t {
    MAXENCODEDSIZEFUN MaxEncodedSize;
    ENCODEFUN         Encode;
    DECODEFUN         Decode;
} codec_t;


static unsigned int LZG_Encode_wrapper(const unsigned char *decBuf,
    unsigned int decSize, unsigned char *encBuf, unsigned int maxEncSize,
    int level, int fast, LZGPROGRESSFUN progressfun, void *userdata)
{
    lzg_encoder_config_t config;
    LZG_InitEncoderConfig(&config);
    config.level = level;
    config.fast = fast;
    config.progressfun = progressfun;
    config.userdata = userdata;
    return LZG_Encode(decBuf, decSize, encBuf, maxEncSize, &config);
}

static void InitCodecLZG(codec_t *c)
{
    c->MaxEncodedSize = LZG_MaxEncodedSize;
    c->Encode = LZG_Encode_wrapper;
    c->Decode = LZG_Decode;
}

static unsigned int MEMCPY_MaxEncodedSize_wrapper(unsigned int insize)
{
    return insize;
}

static unsigned int MEMCPY_Encode_wrapper(const unsigned char *decBuf,
    unsigned int decSize, unsigned char *encBuf, unsigned int UNUSED(maxEncSize),
    int UNUSED(level), int UNUSED(fast), LZGPROGRESSFUN progressfun, void *userdata)
{
    unsigned int i, progress, oldProgress = 999;
    for (i = 0; i < decSize; ++i)
    {
        if (progressfun && !(i & 1023))
        {
            progress = (100 * i) / decSize;
            if (progress != oldProgress)
            {
                progressfun(progress, userdata);
                oldProgress = progress;
            }
        }
        encBuf[i] = decBuf[i];
    }
    if (progressfun)
        progressfun(100, userdata);
    return decSize;
}

static unsigned int MEMCPY_Decode_wrapper(const unsigned char *encBuf,
    unsigned int encSize, unsigned char *decBuf, unsigned int UNUSED(decSize))
{
    unsigned int i;
    for (i = 0; i < encSize; ++i)
        decBuf[i] = encBuf[i];
    return encSize;
}

static void InitCodecMEMCPY(codec_t *c)
{
    c->MaxEncodedSize = MEMCPY_MaxEncodedSize_wrapper;
    c->Encode = MEMCPY_Encode_wrapper;
    c->Decode = MEMCPY_Decode_wrapper;
}

#ifdef USE_ZLIB
static unsigned int ZLIB_MaxEncodedSize_wrapper(unsigned int insize)
{
    // FIXME
    return insize + (insize / 10) + 1000;
}

static unsigned int ZLIB_Encode_wrapper(const unsigned char *decBuf,
    unsigned int decSize, unsigned char *encBuf, unsigned int maxEncSize,
    int level, int UNUSED(fast), LZGPROGRESSFUN progressfun, void *userdata)
{
    int ret;
    unsigned int compressedSize;
    z_stream strm;

    if (progressfun)
        progressfun(0, userdata);

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK) return 0;
    strm.avail_in = decSize;
    strm.next_in = (unsigned char*)decBuf;
    strm.avail_out = maxEncSize;
    strm.next_out = (unsigned char*)encBuf;
    ret = deflate(&strm, Z_FINISH);
    compressedSize = maxEncSize - strm.avail_out;
    deflateEnd(&strm);
    if (ret != Z_STREAM_END) return 0;

    if (progressfun)
        progressfun(100, userdata);

    return compressedSize;
}

static unsigned int ZLIB_Decode_wrapper(const unsigned char *encBuf,
    unsigned int encSize, unsigned char *decBuf, unsigned int decSize)
{
    int ret;
    unsigned int decompressedSize;
    z_stream strm;

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK) return 0;
    strm.avail_in = encSize;
    strm.next_in = (unsigned char*)encBuf;
    strm.avail_out = decSize;
    strm.next_out = (unsigned char*)decBuf;
    ret = inflate(&strm, Z_FINISH);
    decompressedSize = decSize - strm.avail_out;
    inflateEnd(&strm);
    if (ret != Z_STREAM_END) return 0;
    return decompressedSize;
}

static void InitCodecZLIB(codec_t *c)
{
    c->MaxEncodedSize = ZLIB_MaxEncodedSize_wrapper;
    c->Encode = ZLIB_Encode_wrapper;
    c->Decode = ZLIB_Decode_wrapper;
}
#endif

#ifdef USE_BZ2
static unsigned int BZ2_MaxEncodedSize_wrapper(unsigned int insize)
{
    // FIXME
    return insize * 2 + 1000;
}

static unsigned int BZ2_Encode_wrapper(const unsigned char *decBuf,
    unsigned int decSize, unsigned char *encBuf, unsigned int maxEncSize,
    int level, int UNUSED(fast), LZGPROGRESSFUN progressfun, void *userdata)
{
    int ret;
    unsigned int compressedSize;
    bz_stream strm;

    if (progressfun)
        progressfun(0, userdata);

    strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;
    ret = BZ2_bzCompressInit(&strm, level, 0, 0);
    if (ret != BZ_OK) return 0;
    strm.avail_in = decSize;
    strm.next_in = (char*)decBuf;
    strm.avail_out = maxEncSize;
    strm.next_out = (char*)encBuf;
    ret = BZ2_bzCompress(&strm, BZ_FINISH);
    compressedSize = maxEncSize - strm.avail_out;
    BZ2_bzCompressEnd(&strm);
    if (ret != BZ_STREAM_END) return 0;

    if (progressfun)
        progressfun(100, userdata);

    return compressedSize;
}

static unsigned int BZ2_Decode_wrapper(const unsigned char *encBuf,
    unsigned int encSize, unsigned char *decBuf, unsigned int decSize)
{
    int ret;
    unsigned int decompressedSize;
    bz_stream strm;

    strm.bzalloc = NULL;
    strm.bzfree = NULL;
    strm.opaque = NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = BZ2_bzDecompressInit(&strm, 0, 0);
    if (ret != BZ_OK) return 0;
    strm.avail_in = encSize;
    strm.next_in = (char*)encBuf;
    strm.avail_out = decSize;
    strm.next_out = (char*)decBuf;
    ret = BZ2_bzDecompress(&strm);
    decompressedSize = decSize - strm.avail_out;
    BZ2_bzDecompressEnd(&strm);
    if (ret != BZ_STREAM_END) return 0;
    return decompressedSize;
}

static void InitCodecBZ2(codec_t *c)
{
    c->MaxEncodedSize = BZ2_MaxEncodedSize_wrapper;
    c->Encode = BZ2_Encode_wrapper;
    c->Decode = BZ2_Decode_wrapper;
}
#endif

#ifdef USE_LZO
static unsigned int LZO_MaxEncodedSize_wrapper(unsigned int insize)
{
    // FIXME
    return insize + (insize / 10) + 1000;
}

static unsigned int LZO_Encode_wrapper(const unsigned char *decBuf,
    unsigned int decSize, unsigned char *encBuf, unsigned int maxEncSize,
    int level, int UNUSED(fast), LZGPROGRESSFUN progressfun, void *userdata)
{
    lzo_uint wbufSize, compressedSize;
    lzo_byte *workbuf;
    int ret;

    if (progressfun)
        progressfun(0, userdata);

    if (level == 9)
        wbufSize = LZO1X_999_MEM_COMPRESS;
    else
        wbufSize = LZO1X_1_MEM_COMPRESS;

    workbuf = (lzo_byte *)malloc(wbufSize);
    if (!workbuf)
        return 0;

    compressedSize = maxEncSize;
    if (level == 9)
        ret = lzo1x_999_compress(decBuf, decSize, encBuf, &compressedSize, workbuf);
    else
        ret = lzo1x_1_compress(decBuf, decSize, encBuf, &compressedSize, workbuf);
    free(workbuf);
    if (ret != LZO_E_OK)
        return 0;

    if (progressfun)
        progressfun(100, userdata);

    return compressedSize;
}

static unsigned int LZO_Decode_wrapper(const unsigned char *encBuf,
    unsigned int encSize, unsigned char *decBuf, unsigned int decSize)
{
    lzo_uint decompressedSize;

    decompressedSize = decSize;
    if (lzo1x_decompress_safe(encBuf, encSize, decBuf, &decompressedSize, NULL) != LZO_E_OK)
        return 0;

    return decompressedSize;
}

static void InitCodecLZO(codec_t *c)
{
    lzo_init();
    c->MaxEncodedSize = LZO_MaxEncodedSize_wrapper;
    c->Encode = LZO_Encode_wrapper;
    c->Decode = LZO_Decode_wrapper;
}
#endif


/*-- (end of dynamic codec class) -------------------------------------------*/


void ShowUsage(char *prgName)
{
    fprintf(stderr, "Usage: %s [options] file\n", prgName);
    fprintf(stderr, "\nOptions:\n");
    fprintf(stderr, " -1      Use fastest compression\n");
    fprintf(stderr, " -9      Use best compression\n");
    fprintf(stderr, " -s      Do not use the fast method (saves memory, LZG only)\n");
    fprintf(stderr, " -v      Be verbose\n");
    fprintf(stderr, " -m      Perform multiple passes (10)\n");
    fprintf(stderr, " -lzg    Use LZG compression (default).\n");
#ifdef USE_ZLIB
    fprintf(stderr, " -zlib   Use zlib compression.\n");
#endif
#ifdef USE_BZ2
    fprintf(stderr, " -bz2    Use bzip2 compression.\n");
#endif
#ifdef USE_LZO
    fprintf(stderr, " -lzo    Use lzo compression.\n");
#endif
    fprintf(stderr, " -memcpy Use memcpy \"compression\" (raw 1:1 copy).\n");
    fprintf(stderr, "\nDescription:\n");
    fprintf(stderr, "This program will load the given file, compress it, and then decompress it\n");
    fprintf(stderr, "again. The time it takes to do the operations are measured (excluding file\n");
    fprintf(stderr, "I/O etc), and printed to stdout.\n");
}

void ShowProgress(int progress, void *data)
{
    FILE *f = (FILE *)data;
    fprintf(f, "Progress: %d%%   \r", progress);
    fflush(f);
}

int main(int argc, char **argv)
{
    char *inName;
    FILE *inFile;
    size_t fileSize;
    unsigned char *decBuf;
    unsigned int decSize = 0;
    unsigned char *encBuf;
    unsigned int maxEncSize, encSize, t;
    int arg, level, fast, verbose, pass, numPasses, success;
    LZGPROGRESSFUN progressfun = 0;
    codec_t c;

    // Default arguments
    inName = NULL;
    level = 5;
    verbose = 0;
    fast = 1;
    numPasses = 1;
    InitCodecLZG(&c);

    // Get arguments
    for (arg = 1; arg < argc; ++arg)
    {
        if (strcmp("-1", argv[arg]) == 0)
            level = 1;
        else if (strcmp("-2", argv[arg]) == 0)
            level = 2;
        else if (strcmp("-3", argv[arg]) == 0)
            level = 3;
        else if (strcmp("-4", argv[arg]) == 0)
            level = 4;
        else if (strcmp("-5", argv[arg]) == 0)
            level = 5;
        else if (strcmp("-6", argv[arg]) == 0)
            level = 6;
        else if (strcmp("-7", argv[arg]) == 0)
            level = 7;
        else if (strcmp("-8", argv[arg]) == 0)
            level = 8;
        else if (strcmp("-9", argv[arg]) == 0)
            level = 9;
        else if (strcmp("-v", argv[arg]) == 0)
            verbose = 1;
        else if (strcmp("-m", argv[arg]) == 0)
            numPasses = 10;
        else if (strcmp("-s", argv[arg]) == 0)
            fast = 0;
        else if (strcmp("-lzg", argv[arg]) == 0)
            InitCodecLZG(&c);
#ifdef USE_ZLIB
        else if (strcmp("-zlib", argv[arg]) == 0)
            InitCodecZLIB(&c);
#endif
#ifdef USE_BZ2
        else if (strcmp("-bz2", argv[arg]) == 0)
            InitCodecBZ2(&c);
#endif
#ifdef USE_LZO
        else if (strcmp("-lzo", argv[arg]) == 0)
            InitCodecLZO(&c);
#endif
        else if (strcmp("-memcpy", argv[arg]) == 0)
            InitCodecMEMCPY(&c);
        else if (!inName)
            inName = argv[arg];
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

    for (pass = 1; pass <= numPasses; ++pass)
    {
        success = 0;

        // Read input file
        decBuf = (unsigned char*) 0;
        inFile = fopen(inName, "rb");
        if (inFile)
        {
            fseek(inFile, 0, SEEK_END);
            fileSize = (unsigned int) ftell(inFile);
            fseek(inFile, 0, SEEK_SET);
            if (fileSize > 0)
            {
                decSize = (unsigned int) fileSize;
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

        if (decBuf)
        {
            // Determine maximum size of compressed data
            maxEncSize = c.MaxEncodedSize(decSize);

            // Allocate memory for the compressed data
            encBuf = (unsigned char*) malloc(maxEncSize);
            if (encBuf)
            {
                // Compress
                if (verbose)
                    progressfun = ShowProgress;
                StartTimer();
                encSize = c.Encode(decBuf, decSize, encBuf, maxEncSize,
                                    level, fast, progressfun, stderr);
                t = StopTimer();
                if (encSize)
                {
                    fprintf(stdout, "Compression: %d us (%lld KB/s)\n", t,
                                    (decSize * (long long) 977) / t);

                    // Compressed data is now in encBuf, now decompress it...
                    StartTimer();
                    decSize = c.Decode(encBuf, encSize, decBuf, decSize);
                    t = StopTimer();
                    if (decSize)
                    {
                        fprintf(stdout, "Decompression: %d us (%lld KB/s)\n", t,
                                        (decSize * (long long) 977) / t);
                        fprintf(stdout, "Sizes: %d => %d bytes, %d%%\n", decSize, encSize,
                                        (100 * encSize) / decSize);
                        success = 1;
                    }
                    else
                        fprintf(stderr, "Decompression failed!\n");
                }
                else
                    fprintf(stderr, "Compression failed!\n");

                // Free memory when we're done with the compressed data
                free(encBuf);
            }
            else
                fprintf(stderr, "Out of memory!\n");

            // Free memory when we're done with the decompressed data
            free(decBuf);
        }

        if (!success) break;
    }

    return 0;
}

