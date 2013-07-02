/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file enables access to the FSMC peripheral. Normally you don't need to include this file because
 * it will be included as a dependency of something else that uses the FSMC, for example the display drivers.
 */

// fsmc depends on rcc, gpio

#include "config/rcc.h"
#include "config/gpio.h"

// generic peripheral includes

#include "fsmc/FsmcPeripheral.h"

// device-specific peripheral includes

#if defined(STM32PLUS_F1)
  #include "fsmc/f1/FsmcPeripheral.h"
#elif defined(STM32PLUS_F4)
  #include "fsmc/f4/FsmcPeripheral.h"
#endif

// includes for the timings

#include "fsmc/FsmcAddressDataSetupTiming.h"
#include "fsmc/Fsmc8080LcdTiming.h"
#include "fsmc/FsmcNandTiming.h"
#include "fsmc/FsmcSramTiming.h"

// includes for memories

#include "fsmc/FsmcSram.h"
#include "fsmc/FsmcNand.h"
