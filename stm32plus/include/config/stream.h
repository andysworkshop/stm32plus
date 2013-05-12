/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this file to get access to the general purpose stream I/O classes. There are implementations
 * of streams for byte arrays, circular buffers, LZG decompression and linear buffers. Some plumbing
 * classes implement connected and buffered streams.
 */

// stream depends on timing, memblock

#include "config/timing.h"
#include "memory/Memblock.h"

// includes for the feature

#include "stream/StreamBase.h"
#include "stream/InputStream.h"
#include "stream/OutputStream.h"
#include "stream/BufferedInputOutputStream.h"
#include "stream/ByteArrayOutputStream.h"
#include "stream/CircularBufferInputOutputStream.h"
#include "stream/LzgDecompressionInputStream.h"
#include "stream/ConnectedInputOutputStream.h"
#include "stream/LinearBufferInputOutputStream.h"
