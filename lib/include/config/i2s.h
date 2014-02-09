/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This config file provides access to the I2S peripheral functionality. DMA and interrupt based access to the
 * peripheral are supported. All the I2S peripherals are supported with all the pin configurations.
 */

// i2s depends on rcc, gpio, event, interrupts, dma, spi

#include "config/rcc.h"
#include "config/gpio.h"
#include "config/event.h"
#include "config/dma.h"
#include "config/spi.h"

// device-specific pin initialiser

#if defined(STM32PLUS_F1)
  #include "i2s/f1/I2SPinInitialiser.h"
#elif defined(STM32PLUS_F4)
  #include "i2s/f4/I2SAlternateFunctionMapper.h"
  #include "i2s/f4/I2SPinInitialiser.h"
#elif defined(STM32PLUS_F0)
  #include "i2s/f0/I2SAlternateFunctionMapper.h"
  #include "i2s/f0/I2SPinInitialiser.h"
#endif

// generic peripheral includes

#include "i2s/I2S.h"
#include "i2s/I2SPeripheral.h"

// generic feature includes

#include "i2s/features/I2SFeatureBase.h"
#include "i2s/features/I2SInterruptFeature.h"

// generic peripheral includes

#include "i2s/I2S.h"
#include "i2s/I2SPeripheral.h"
#include "i2s/I2S2.h"
#include "i2s/I2S3.h"

// enable all interrupts

#define USE_I2S2_INTERRUPT
#define USE_I2S3_INTERRUPT
