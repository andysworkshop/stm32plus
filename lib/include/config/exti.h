/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This config file provides access to the EXTI (external interrupt) peripheral functionality. EXTI is
 * typically the best choice for interfacing any peripheral that provides an "I've been activated" line
 * that changes state when it has something to tell you. Push-buttons are another good example.
 */

// exti depends on gpio, event, nvic

#include "config/gpio.h"
#include "config/event.h"
#include "config/nvic.h"

// enable all interrupts

#define USE_EXTI0_INTERRUPT
#define USE_EXTI0_1_INTERRUPT
#define USE_EXTI1_INTERRUPT
#define USE_EXTI2_INTERRUPT
#define USE_EXTI2_3_INTERRUPT
#define USE_EXTI3_INTERRUPT
#define USE_EXTI4_INTERRUPT
#define USE_EXTI4_15_INTERRUPT
#define USE_EXTI9_5_INTERRUPT
#define USE_EXTI15_10_INTERRUPT
#define USE_EXTI_PVD_INTERRUPT
#define USE_EXTI_RTC_ALARM_INTERRUPT
#define USE_EXTI_RTC_INTERRUPT
#define USE_EXTI_USB_FS_WAKEUP_INTERRUPT
#define USE_EXTI_OTG_FS_WAKEUP_INTERRUPT      // CL only
#define USE_EXTI_ETH_WAKEUP_INTERRUPT
#define USE_EXTI_USB_HS_WAKEUP_INTERRUPT
#define USE_EXTI_TAMP_STAMP_INTERRUPT
#define USE_EXTI_RTC_WAKEUP_INTERRUPT

// common peripheral includes

#include "exti/ExtiPeripheralBase.h"

// device-specific includes

#if defined(STM32PLUS_F1)
  #include "exti/f1/ExtiInterruptEnabler.h"
  #include "exti/f1/ExtiPeripheral.h"
#elif defined(STM32PLUS_F4)
  #include "exti/f4/ExtiInterruptEnabler.h"
  #include "exti/f4/ExtiPeripheral.h"
#elif defined(STM32PLUS_F0)
  #include "exti/f0/ExtiInterruptEnabler.h"
  #include "exti/f0/ExtiPeripheral.h"
#endif

