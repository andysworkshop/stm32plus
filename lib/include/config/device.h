#/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file gives access to the low-level block device drivers and streams. You normally don't have to include
 * this config file as it's included for you as a dependency of something else such as the filesystem drivers.
 */

// device depends on filesystem MBR, memory copy, memblock, stream

#include "filesystem/MbrPartition.h"
#include "filesystem/Mbr.h"
#include "memory/MEM_DataCopy.h"
#include "memory/Memblock.h"
#include "config/stream.h"

// includes for the feature

#include "device/BlockDevice.h"
#include "device/CachedBlockDevice.h"

// includes for the extra classes

#include "device/BlockDeviceOutputStream.h"
#include "device/BlockDeviceInputStream.h"
