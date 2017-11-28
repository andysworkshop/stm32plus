/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

/**
 * @file
 * This file provides access to the STM32 timer peripherals. The timers are incredibly configurable and
 * very powerful peripherals that can be used on their own or linked in various ways to other peripherals
 * on the STM32 where timing might be useful.
 *
 * Access to all the timer peripherals is provided and feature classes are there for you to pick and choose
 * what you need from your timer. There's an awful lot in here, see the examples for a good run-down of
 * what you can do.
 */

// timer depends on gpio, event, interrupts, nvic, rtc, exti, scoped_ptr

#include "config/gpio.h"
#include "config/nvic.h"
#include "config/rtc.h"
#include "config/event.h"
#include "config/exti.h"
#include "memory/scoped_ptr.h"

// generic peripheral and feature includes

#include "timer/TimerEventSource.h"
#include "timer/Timer.h"
#include "timer/TimerPeripheral.h"

#include "timer/features/TimerFeatureBase.h"
#include "timer/features/TimerInterruptFeatureEnabler.h"
#include "timer/features/TimerChannelFeatureBase.h"

// generic timer channel feature includes

#include "timer/features/channel/TimerChannelOCPolarityFeature.h"
#include "timer/features/channel/TimerChannelOCNPolarityFeature.h"
#include "timer/features/channel/TimerChannelOCModeFeature.h"
#include "timer/features/channel/TimerChannelOCPulseFeature.h"
#include "timer/features/channel/TimerChannelOCIdleStateFeature.h"
#include "timer/features/channel/TimerChannelOCNIdleStateFeature.h"

#include "timer/features/channel/TimerChannelICPolarityFeature.h"
#include "timer/features/channel/TimerChannelICSelectionFeature.h"
#include "timer/features/channel/TimerChannelICPrescalerFeature.h"
#include "timer/features/channel/TimerChannelICFilterFeature.h"

// generic feature includes

#include "timer/features/TimerChannelFeature.h"
#include "timer/features/TimerChannel1Feature.h"
#include "timer/features/TimerChannel2Feature.h"
#include "timer/features/TimerChannel3Feature.h"
#include "timer/features/TimerChannel4Feature.h"
#include "timer/features/TimerExternalMode1ClockFeature.h"
#include "timer/features/TimerInternalTriggerClockFeature.h"
#include "timer/features/TimerMasterFeature.h"
#include "timer/features/TimerSlaveFeature.h"
#include "timer/features/TimerEncoderFeature.h"
#include "timer/features/TimerBreakFeature.h"

// generic peripheral includes

#include "timer/TimerPeripheral.h"
#include "timer/AdvancedControlTimer.h"
#include "timer/GeneralPurposeTimer.h"
#include "timer/BasicTimer.h"

#include "timer/Timer1.h"
#include "timer/Timer2.h"
#include "timer/Timer3.h"

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F0) || defined(STM32PLUS_F4_HAS_TIM6_7_8_12_13_14)

  #include "timer/Timer6.h"
  #include "timer/Timer14.h"

#endif

// MCU-specific timer includes

#if defined(STM32PLUS_F1)

  #include "timer/f1/Timer4.h"
  #include "timer/f1/Timer5.h"
  #include "timer/f1/Timer7.h"
  #include "timer/f1/Timer8.h"
  #include "timer/f1/Timer9.h"
  #include "timer/f1/Timer10.h"
  #include "timer/f1/Timer11.h"
  #include "timer/f1/Timer12.h"
  #include "timer/f1/Timer13.h"

#elif defined(STM32PLUS_F4)

  #include "timer/f4/Timer4.h"
  #include "timer/f4/Timer5.h"
  #include "timer/f4/Timer9.h"
  #include "timer/f4/Timer10.h"
  #include "timer/f4/Timer11.h"

#if defined(STM32PLUS_F4_HAS_TIM6_7_8_12_13_14)

  #include "timer/f4/Timer7.h"
  #include "timer/f4/Timer8.h"
  #include "timer/f4/Timer12.h"
  #include "timer/f4/Timer13.h"

#endif

#elif defined(STM32PLUS_F0)

  #include "timer/f0/Timer15.h"
  #include "timer/f0/Timer16.h"
  #include "timer/f0/Timer17.h"

#endif

// enable all interrupts

#define USE_TIM1_BRK_INTERRUPT
#define USE_TIM1_UP_INTERRUPT
#define USE_TIM1_TRG_COM_INTERRUPT
#define USE_TIM1_CC_INTERRUPT
#define USE_TIM1_UP_TIM16_INTERRUPT
#define USE_TIM1_BRK_TIM15_INTERRUPT
#define USE_TIM1_TRG_COM_TIM17_INTERRUPT
#define USE_TIM1_BRK_UP_TRG_COM_INTERRUPT

#define USE_TIM8_BRK_INTERRUPT
#define USE_TIM8_UP_INTERRUPT
#define USE_TIM8_TRG_COM_INTERRUPT
#define USE_TIM8_CC_INTERRUPT

#define USE_TIM2_INTERRUPT
#define USE_TIM3_INTERRUPT
#define USE_TIM4_INTERRUPT
#define USE_TIM5_INTERRUPT
#define USE_TIM6_INTERRUPT
#define USE_TIM7_INTERRUPT
#define USE_TIM9_INTERRUPT
#define USE_TIM10_INTERRUPT
#define USE_TIM11_INTERRUPT
#define USE_TIM12_INTERRUPT
#define USE_TIM13_INTERRUPT
#define USE_TIM14_INTERRUPT
#define USE_TIM15_INTERRUPT
#define USE_TIM16_INTERRUPT
#define USE_TIM17_INTERRUPT

// device-specific feature includes

#if defined(STM32PLUS_F1_HD)
  #include "timer/features/f1/hd/TimerInterruptFeature.h"
#elif defined(STM32PLUS_F1_CL)
  #include "timer/features/f1/cl/TimerInterruptFeature.h"
#elif defined(STM32PLUS_F1_MD)
  #include "timer/features/f1/md/TimerInterruptFeature.h"
#elif defined(STM32PLUS_F1_MD_VL)
  #include "timer/features/f1/mdvl/TimerInterruptFeature.h"
#elif defined(STM32PLUS_F1_XL)
  #include "timer/features/f1/xl/TimerInterruptFeature.h"
#endif

#if defined(STM32PLUS_F1)

  #include "timer/features/f1/TimerInternalClockFeature.h"
  #include "timer/features/f1/Timer1GpioFeature.h"
  #include "timer/features/f1/Timer2GpioFeature.h"
  #include "timer/features/f1/Timer3GpioFeature.h"
  #include "timer/features/f1/Timer4GpioFeature.h"
  #include "timer/features/f1/Timer5GpioFeature.h"
  #include "timer/features/f1/Timer8GpioFeature.h"
  #include "timer/features/f1/Timer9GpioFeature.h"
  #include "timer/features/f1/Timer10GpioFeature.h"
  #include "timer/features/f1/Timer11GpioFeature.h"
  #include "timer/features/f1/Timer13GpioFeature.h"
  #include "timer/features/f1/Timer14GpioFeature.h"

#elif defined(STM32PLUS_F4)

  #include "timer/f4/TimerAlternateFunctionMapper.h"
  #include "timer/features/f4/TimerInterruptFeature.h"
  #include "timer/features/f4/Timer1GpioFeature.h"
  #include "timer/features/f4/Timer2GpioFeature.h"
  #include "timer/features/f4/Timer3GpioFeature.h"
  #include "timer/features/f4/Timer4GpioFeature.h"
  #include "timer/features/f4/Timer5GpioFeature.h"
  #include "timer/features/f4/Timer9GpioFeature.h"
  #include "timer/features/f4/Timer10GpioFeature.h"
  #include "timer/features/f4/Timer11GpioFeature.h"

#if defined(STM32PLUS_F4_HAS_TIM6_7_8_12_13_14)

  #include "timer/features/f4/Timer8GpioFeature.h"
  #include "timer/features/f4/Timer13GpioFeature.h"
  #include "timer/features/f4/Timer14GpioFeature.h"

#endif

  #include "timer/features/f4/Timer2RemapEthernetPtpFeature.h"
  #include "timer/features/f4/Timer2RemapUsbFullSpeedSofFeature.h"
  #include "timer/features/f4/Timer2RemapUsbHighSpeedSofFeature.h"
  #include "timer/features/f4/Timer5RemapLsiFeature.h"
  #include "timer/features/f4/Timer5RemapLseFeature.h"
  #include "timer/features/f4/Timer5RemapRtcFeature.h"
  #include "timer/features/f4/Timer11RemapHseFeature.h"
  #include "timer/features/f4/TimerInternalClockFeature.h"

#elif defined(STM32PLUS_F0)

  #include "timer/f0/TimerAlternateFunctionMapper.h"
  #include "timer/features/f0/TimerInterruptFeature.h"
  #include "timer/features/f0/Timer1GpioFeature.h"
  #include "timer/features/f0/Timer2GpioFeature.h"
  #include "timer/features/f0/Timer3GpioFeature.h"
  #include "timer/features/f0/Timer14GpioFeature.h"
  #include "timer/features/f0/Timer15GpioFeature.h"
  #include "timer/features/f0/Timer16GpioFeature.h"
  #include "timer/features/f0/Timer17GpioFeature.h"
  #include "timer/features/f0/Timer14RemapGpioFeature.h"
  #include "timer/features/f0/Timer14RemapHseDiv32Feature.h"
  #include "timer/features/f0/Timer14RemapMcoFeature.h"
  #include "timer/features/f0/Timer14RemapRtcClkFeature.h"
  #include "timer/features/f0/TimerInternalClockFeature.h"

#endif
