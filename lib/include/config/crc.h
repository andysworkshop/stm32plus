/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file gets you access to the CRC peripheral functionality. Big-endian and little-endian
 * calculation is supported.
 */

// CRC depends on output stream

#include "config/stream.h"

// generic peripheral includes

#include "crc/CrcBase.h"
#include "crc/CrcPeripheral.h"
#include "crc/BigEndianCrc.h"
#include "crc/LittleEndianCrc.h"

// utility classes

#include "crc/CrcOutputStream.h"
