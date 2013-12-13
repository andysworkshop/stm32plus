/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This is an internal config file that is included by any peripheral that has a clock, i.e. all of them. Methods
 * are provided to allow clocks to be stopped and started and to interrogate the bus speeds and multipliers.
 */

// generic includes

#include "include/rcc/ClockControl.h"
#include "include/rcc/RccBusInformation.h"
#include "include/rcc/PeripheralBusSpeed.h"

// device-specific includes

#if defined(STM32PLUS_F1)

  #include "include/rcc/f1/ClockControl.h"
  #include "include/rcc/f1/PeripheralBusSpeed.h"

#elif defined(STM32PLUS_F4)

  #include "include/rcc/f4/ClockControl.h"
  #include "include/rcc/f4/PeripheralBusSpeed.h"

#elif defined(STM32PLUS_F0)

  #include "include/rcc/f0/ClockControl.h"

#endif

