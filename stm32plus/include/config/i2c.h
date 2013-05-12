/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * This file provides access to the I2C peripheral. All the peripherals are exposed with multiple pin configurations
 * available. DMA and interrupt-driven access are both supported.
 */

// i2c depends on rcc, gpio, observer, interrupts, dma, timing

#include "config/rcc.h"
#include "config/gpio.h"
#include "config/observer.h"
#include "config/dma.h"
#include "config/timing.h"

// generic peripheral includes

#include "i2c/I2CPinInitialiser.h"
#include "i2c/I2C.h"
#include "i2c/I2CPeripheral.h"

// generic feature includes

#include "i2c/features/I2CFeatureBase.h"
#include "i2c/features/I2CMasterPollingFeature.h"
#include "i2c/features/I2CSingleByteMasterPollingFeature.h"
#include "i2c/features/I2CTwoByteMasterPollingFeature.h"
#include "i2c/features/I2CInterruptFeature.h"

// generic peripheral includes

#include "i2c/I2C.h"
#include "i2c/I2CPeripheral.h"
#include "i2c/I2C1.h"
#include "i2c/I2C2.h"

// enable all interrupts

#define USE_I2C1_INTERRUPT
#define USE_I2C2_INTERRUPT

// device specific includes

#if defined(STM32PLUS_F1)

#elif defined(STM32PLUS_F4)

	#define USE_I2C3_INTERRUPT

	#include "i2c/features/f4/I2CInterruptFeature.h"

	#include "i2c/f4/I2C2Remap.h"
	#include "i2c/f4/I2C3.h"

#endif
