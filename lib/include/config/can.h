/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// dependencies

#include "config/nvic.h"
#include "config/rcc.h"
#include "config/gpio.h"
#include "config/event.h"
#include "util/Meta.h"

// device-specific pin initialiser

#if defined(STM32PLUS_F1)
  #include "can/f1/CanPinInitialiser.h"
#elif defined(STM32PLUS_F4)
  #include "can/f4/CanAlternateFunctionMapper.h"
  #include "can/f4/CanPinInitialiser.h"
#endif

// generic peripheral includes

#include "can/CanEventSource.h"
#include "can/Can.h"
#include "can/CanPeripheral.h"
#include "can/CanFeatureBase.h"

// device-specific peripheral includes

#if defined(STM32PLUS_F1)
  #include "can/f1/Can1.h"
#elif defined(STM32PLUS_F4)
  #include "can/f4/Can1.h"
  #include "can/f4/Can2.h"
#endif

#define USE_CAN1_INTERRUPT
#define USE_CAN2_INTERRUPT

// generic features

#include "can/features/Can1FilterBypassFeature.h"
#include "can/features/Can2FilterBypassFeature.h"
#include "can/features/CanSilentModeFeature.h"
#include "can/features/CanLoopbackModeFeature.h"

// device-specific features

#if defined(STM32PLUS_F1)
  #include "can/features/f1/hd,md/Can1InterruptFeature.h"
#elif defined(STM32PLUS_F4)
  #include "can/features/f4/Can1InterruptFeature.h"
  #include "can/features/f4/Can2InterruptFeature.h"
#endif
