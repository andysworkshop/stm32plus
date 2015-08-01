/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This file provides access to the I2C peripheral. All the peripherals are exposed with multiple pin configurations
 * available. DMA and interrupt-driven access are both supported.
 */

// i2c depends on rcc, gpio, event, interrupts, dma, timing

#include "config/rcc.h"
#include "config/gpio.h"
#include "config/event.h"
#include "config/dma.h"
#include "config/timing.h"

// generic peripheral includes

#include "i2c/I2CPinInitialiser.h"
#include "i2c/I2CEventSource.h"

#if defined(STM32PLUS_F0)
  #include "i2c/f0/I2C.h"
  #include "i2c/f0/I2CPeripheral.h"
#elif defined(STM32PLUS_F1) || defined(STM32PLUS_F4)
  #include "i2c/f1,f4/I2C.h"
  #include "i2c/f1,f4/I2CPeripheral.h"
#endif

// generic feature includes

#include "i2c/features/I2CFeatureBase.h"
#include "i2c/features/I2CIoFeature.h"
#include "i2c/features/I2CMasterPollingFeature.h"
#include "i2c/features/I2CSingleByteMasterPollingFeature.h"
#include "i2c/features/I2CTwoByteMasterPollingFeature.h"
#include "i2c/features/I2CInterruptFeature.h"

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)
#include "i2c/features/f1,f4/I2CSlaveFeature.h"
#endif

// includes for the alternate function mappings

#if defined(STM32PLUS_F4)
  #include "i2c/f4/I2CAlternateFunctionMapper.h"
#elif defined(STM32PLUS_F0)
  #include "i2c/f0/I2CAlternateFunctionMapper.h"
#endif

// generic peripheral includes

#include "i2c/I2C2.h"

#if defined(STM32PLUS_F0)
  #include "i2c/f0/I2C1.h"
#elif defined(STM32PLUS_F1)
  #include "i2c/f1/I2C1.h"
#elif defined(STM32PLUS_F4)
  #include "i2c/f4/I2C1.h"
#endif

// enable all interrupts

#define USE_I2C1_INTERRUPT
#define USE_I2C2_INTERRUPT

// device specific includes

#if defined(STM32PLUS_F0)

  #include "i2c/features/f0/I2CSecondSlaveFeature.h"
  #include "i2c/f0/I2C2Remap.h"

#elif defined(STM32PLUS_F1)

  #include "i2c/features/f1/I2CSecondSlaveFeature.h"

#elif defined(STM32PLUS_F4)

  #define USE_I2C3_INTERRUPT

  #include "i2c/features/f4/I2CInterruptFeature.h"
  #include "i2c/features/f4/I2CSecondSlaveFeature.h"

  #include "i2c/f4/I2C2Remap.h"
  #include "i2c/f4/I2C3.h"

#endif
