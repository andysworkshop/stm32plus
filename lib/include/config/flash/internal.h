/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file gets you access to the internal flash memory device. Please note that the internal flash
 * on all STM32 models is rated for 10000 erase/program operations on each page.
 */


// device specific implementation

#if defined(STM32PLUS_F0)
  #include "flash/internal/f0/InternalFlashPeripheral.h"
#endif

// the main classes

#include "flash/internal/InternalFlashDevice.h"

// device specific features

#if defined(STM32PLUS_F0)
  #include "flash/internal/f0/features/InternalFlashWriteFeature.h"
#endif
