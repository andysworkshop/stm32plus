/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * Include this file to get access to the real time clock functionality. These peripherals are significantly different
 * on the F1 and F4. Support is also provided for the I2C-based DS1307 device.
 */

// rtc depends on rcc, nvic, event, exti, timer

#include "config/rcc.h"
#include "config/nvic.h"
#include "config/event.h"
#include "config/exti.h"
#include "config/timer.h"

// device-specifc includes

#if defined(STM32PLUS_F1)
  #include "rtc/f1/RtcBase.h"
#elif defined(STM32PLUS_F4)
  #include "rtc/f4/RtcBase.h"
#elif defined(STM32PLUS_F0)
  #include "rtc/f0/RtcBase.h"
#else
  #error Unsupported MCU
#endif

// generic feature includes

#include "rtc/Rtc.h"
#include "rtc/features/RtcFeatureBase.h"
#include "rtc/features/RtcConstantLsiFrequencyProvider.h"

// device specific feature includes

#if defined(STM32PLUS_F1)

  #define USE_RTC_INTERRUPT

  #include "rtc/features/f1/RtcAlarmInterruptFeature.h"
  #include "rtc/features/f1/RtcLseClockFeature.h"
  #include "rtc/features/f1/RtcOverflowInterruptFeature.h"
  #include "rtc/features/f1/RtcSecondInterruptFeature.h"
  #include "rtc/features/f1/RtcTamperOutputFeature.h"
  #include "rtc/features/f1/RtcLsiClockFeature.h"

#elif defined(STM32PLUS_F4)

  #include "rtc/features/f4/RtcAlarmInterruptFeature.h"
  #include "rtc/features/f4/RtcLseClockFeature.h"
  #include "rtc/features/f4/RtcLsiClockFeature.h"
  #include "rtc/features/f4/RtcSecondInterruptFeature.h"
  #include "rtc/features/f4/RtcMeasuredLsiFrequencyProvider.h"

#elif defined(STM32PLUS_F0)

  #include "rtc/features/f0/RtcAlarmInterruptFeature.h"
  #include "rtc/features/f0/RtcLseClockFeature.h"
  #include "rtc/features/f0/RtcLsiClockFeature.h"
  #include "rtc/features/f0/RtcHseDiv32ClockFeature.h"
  #include "rtc/features/f0/RtcMeasuredLsiFrequencyProvider.h"

#endif

// external device support

#include "rtc/DS1307/DS1307.h"
