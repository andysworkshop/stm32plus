/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
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

// timer depends on gpio, observer, interrupts, nvic, rtc

#include "config/gpio.h"
#include "config/observer.h"
#include "config/nvic.h"
#include "config/rtc.h"

// generic peripheral and feature includes

#include "timer/Timer.h"
#include "timer/TimerPeripheral.h"

#include "timer/features/TimerFeatureBase.h"
#include "timer/features/TimerInterruptFeatureEnabler.h"
#include "timer/features/TimerChannelFeatureBase.h"
#include "timer/features/NullTimerGpio.h"

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
#include "timer/features/TimerInternalClockFeature.h"

// generic peripheral includes

#include "timer/TimerPeripheral.h"
#include "timer/AdvancedControlTimer.h"
#include "timer/GeneralPurposeTimer.h"
#include "timer/BasicTimer.h"

#include "timer/Timer1.h"
#include "timer/Timer2.h"
#include "timer/Timer3.h"
#include "timer/Timer4.h"
#include "timer/Timer5.h"
#include "timer/Timer6.h"
#include "timer/Timer7.h"
#include "timer/Timer8.h"
#include "timer/Timer9.h"
#include "timer/Timer10.h"
#include "timer/Timer11.h"
#include "timer/Timer12.h"
#include "timer/Timer13.h"
#include "timer/Timer14.h"

// enable all interrupts

#define USE_TIM1_BRK_INTERRUPT
#define USE_TIM1_UP_INTERRUPT
#define USE_TIM1_TRG_COM_INTERRUPT
#define USE_TIM1_CC_INTERRUPT

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

// device-specific feature includes

#if defined(STM32PLUS_F1)

  #include "timer/features/f1/TimerInterruptFeature.h"
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

	#include "timer/features/f4/TimerInterruptFeature.h"
	#include "timer/features/f4/Timer1GpioFeature.h"
	#include "timer/features/f4/Timer2GpioFeature.h"
	#include "timer/features/f4/Timer3GpioFeature.h"
	#include "timer/features/f4/Timer4GpioFeature.h"
	#include "timer/features/f4/Timer5GpioFeature.h"
	#include "timer/features/f4/Timer8GpioFeature.h"
	#include "timer/features/f4/Timer9GpioFeature.h"
	#include "timer/features/f4/Timer10GpioFeature.h"
	#include "timer/features/f4/Timer11GpioFeature.h"
	#include "timer/features/f4/Timer13GpioFeature.h"
	#include "timer/features/f4/Timer14GpioFeature.h"
	#include "timer/features/f4/Timer2RemapEthernetPtpFeature.h"
	#include "timer/features/f4/Timer2RemapUsbFullSpeedSofFeature.h"
	#include "timer/features/f4/Timer2RemapUsbHighSpeedSofFeature.h"
	#include "timer/features/f4/Timer5RemapLsiFeature.h"
	#include "timer/features/f4/Timer5RemapLseFeature.h"
	#include "timer/features/f4/Timer5RemapRtcFeature.h"
	#include "timer/features/f4/Timer11RemapHseFeature.h"

#endif
