/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file gets you access to the STM32 DAC peripheral. Functionality is provided to access both DAC channels
 * either synchronously or via DMA.
 *
 * A driver is also provided for the CS43L22 peripheral that can be found onboard the STM32F4DISCOVERY board
 * sold by ST Microelectronics.
 */

// dac depends on I2C, I2S, rcc, dma

#include "config/i2c.h"
#include "config/i2s.h"
#include "config/rcc.h"
#include "config/dma.h"

// peripheral includes

#include "dac/Dac.h"
#include "dac/DacPinInitialiser.h"
#include "dac/DacPeripheral.h"

// feature includes

#include "dac/features/DacFeatureBase.h"
#include "dac/features/DacDataAlignmentFeature.h"

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)
  #include "dac/features/f1,f4/DacDualChannelFeature.h"
#endif

// dac channels

#include "dac/Dac1.h"
#include "dac/Dac2.h"

// specific device includes

#include "dac/CS43L22/CS43L22ControlI2C.h"
#include "dac/CS43L22/NullCS43L22DataPeripheral.h"
#include "dac/CS43L22/CS43L22.h"
