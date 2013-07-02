/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the flash memory interfaces. Support is provided for the Atmel AT45DB161D
 * serial flash device and the Samsung K9F1G08Q0C NAND flash device.
 */

// flash depends on fsmc, device, millisecond timer, spi

#include "config/device.h"
#include "config/fsmc.h"
#include "config/spi.h"
#include "timing/MillisecondTimer.h"

// includes for the devices

#include "flash/AT45DB161D.h"
#include "flash/K9F1G08Q0C.h"
