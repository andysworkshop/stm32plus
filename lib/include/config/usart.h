/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

/**
 * @file
 * Include this file to get access to the USART peripherals. All peripherals are exposed with all the
 * alternate-function pin mappings. Access via interrupts and DMA are both supported and there's an
 * input and an output stream to help access in polling mode.
 */

// usart depends on rcc, gpio, stream, interrupts, dma

#include "config/rcc.h"
#include "config/gpio.h"
#include "config/stream.h"
#include "config/dma.h"

// device-specific pin initialiser

#if defined(STM32PLUS_F1)

  #include "usart/f1/UsartPinInitialiser.h"

#elif defined(STM32PLUS_F4)

  #include "usart/f4/UsartAlternateFunctionMapper.h"
  #include "usart/f4/UsartPinInitialiser.h"

#endif

// generic peripheral includes

#include "usart/UsartEventSource.h"
#include "usart/Usart.h"
#include "usart/UsartPeripheral.h"

// generic feature includes

#include "usart/features/UsartFeatureBase.h"

// generic peripheral includes

#include "usart/Usart1.h"
#include "usart/Usart2.h"
#include "usart/Usart3.h"
#include "usart/Uart4.h"
#include "usart/Uart5.h"

// enable all interrupt handlers

#define USE_USART1_INTERRUPT
#define USE_USART2_INTERRUPT
#define USE_USART3_INTERRUPT
#define USE_UART4_INTERRUPT
#define USE_UART5_INTERRUPT

// device specific includes

#if defined(STM32PLUS_F1)

  #include "usart/features/f1/UsartInterruptFeature.h"

#elif defined(STM32PLUS_F4)

  #define USE_USART6_INTERRUPT

  #include "usart/f4/Usart6.h"

  #include "usart/features/f4/UsartInterruptFeature.h"

#endif

// utility classes

#include "usart/UsartPollingInputStream.h"
#include "usart/UsartPollingOutputStream.h"
