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

// internal flash depends on CRC

#include "config/crc.h"

// device specific includes

#if defined(STM32PLUS_F0)

  #include "flash/internal/f0/InternalFlashPeripheral.h"
  #include "flash/internal/f0/InternalFlashDevice.h"

#elif defined(STM32PLUS_F1)

  #include "flash/internal/f1/InternalFlashPeripheral.h"
  #include "flash/internal/f1/InternalFlashDevice.h"

#elif defined(STM32PLUS_F4)

  #include "flash/internal/f4/InternalFlashVoltageRange.h"
  #include "flash/internal/f4/InternalFlashSectorMap.h"
  #include "flash/internal/f4/InternalFlashPeripheral.h"
  #include "flash/internal/f4/InternalFlashDevice.h"

#endif

// generic feature includes

#include "flash/internal/features/InternalFlashFeatureBase.h"
#include "flash/internal/features/InternalFlashLockFeature.h"

// device specific features

#if defined(STM32PLUS_F0)

  #include "flash/internal/f0/features/InternalFlashWriteFeature.h"

#elif defined(STM32PLUS_F1)

  #include "flash/internal/f1/features/InternalFlashWriteFeature.h"

#elif defined(STM32PLUS_F4)

  #include "flash/internal/f4/features/InternalFlashWriteFeature.h"

#endif

// general utilities

#include "flash/internal/InternalFlashWordWriter.h"
#include "flash/internal/InternalFlashSettingsStorage.h"
