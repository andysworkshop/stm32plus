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
#endif

// generic peripheral includes

#include "can/CanEventSource.h"
#include "can/Can.h"
#include "can/CanPeripheral.h"
#include "can/CanFeatureBase.h"

// device-specific peripheral includes

#if defined(STM32PLUS_F1)
  #include "can/f1/Can1.h"
#endif

#define USE_CAN1_INTERRUPT

// generic features

#include "can/features/CanFilterBypassFeature.h"
#include "can/features/CanSilentModeFeature.h"
#include "can/features/CanLoopbackModeFeature.h"

// device-specific features

#if defined(STM32PLUS_F1)
  #include <can/features/f1/md,hd/CanInterruptFeature.h>
#endif
