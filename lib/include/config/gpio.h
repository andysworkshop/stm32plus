/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this config file to use the raw GPIO port and pin functionality. Just about every peripheral with an
 * external interface includes this as a dependency so there's little chance that you'll need to include it directly.
 */

// dependencies

#include "util/BitHacks.h"

// generic peripheral includes

#include "gpio/GpioPortBase.h"
#include "gpio/GpioAlternateFunctionMapper.h"

// device-specific feature includes

#if defined(STM32PLUS_F1)

  #include "gpio/f1/GpioPinMap.h"
  #include "gpio/f1/Gpio.h"
  #include "gpio/GpioPinRef.h"
  #include "gpio/GpioIterator.h"
  #include "gpio/f1/GpioPort.h"
  #include "gpio/f1/features/DigitalOutputFeature.h"
  #include "gpio/f1/features/DigitalInputFeature.h"
  #include "gpio/f1/features/AlternateFunctionFeature.h"
  #include "gpio/f1/features/AnalogInputFeature.h"
  #include "gpio/f1/GpioAlternateFunctionMapper.h"
  #include "gpio/f1/GpioPorts.h"

#elif defined(STM32PLUS_F4)

  #include "gpio/f4/GpioPinMap.h"
  #include "gpio/f4/Gpio.h"
  #include "gpio/GpioPinRef.h"
  #include "gpio/GpioIterator.h"
  #include "gpio/f4/GpioPort.h"
  #include "gpio/f4/features/DigitalOutputFeature.h"
  #include "gpio/f4/features/DigitalInputFeature.h"
  #include "gpio/f4/features/AlternateFunctionFeature.h"
  #include "gpio/f4/features/AnalogInputFeature.h"
  #include "gpio/f4/GpioPorts.h"


#elif defined(STM32PLUS_F0)

  #include "gpio/f0/GpioPinMap.h"
  #include "gpio/f0/Gpio.h"
  #include "gpio/GpioPinRef.h"
  #include "gpio/GpioIterator.h"
  #include "gpio/f0/GpioPort.h"
  #include "gpio/f0/features/DigitalOutputFeature.h"
  #include "gpio/f0/features/DigitalInputFeature.h"
  #include "gpio/f0/features/AlternateFunctionFeature.h"
  #include "gpio/f0/features/AnalogInputFeature.h"
  #include "gpio/f0/GpioPorts.h"

#endif

// generic utility includes

#include "gpio/GpioPinInitialiser.h"
