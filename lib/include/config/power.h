/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this config file to get access to the power management functionality of the STM32. Low power modes
 * allow you to put the STM32 into its sleep or stop mode.
 */

// power depends on rcc

#include "config/rcc.h"

// generic includes for the peripheral

#if defined(STM32PLUS_F0)
  #include "pwr/f0/LowPowerModes.h"
#elif defined(STM32PLUS_F1) || defined(STM32PLUS_F4)
  #include "pwr/f1,f4/LowPowerModes.h"
#endif
