/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * Feature class to configure the timer as using the internal clock as
   * its source.
   */

  class TimerInternalClockFeature : public TimerFeatureBase {
    public:
      TimerInternalClockFeature(Timer& timer);
  };


  /**
   * Work out the timer clock from the RCC bus prescaler
   */

  inline TimerInternalClockFeature::TimerInternalClockFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    uint32_t timerClock;

    switch(RCC->CFGR & RCC_CFGR_PPRE) {

      case RCC_CFGR_PPRE_DIV1:              // additional multiplier = 1
      case RCC_CFGR_PPRE_DIV2:              // additional multiplier = 2
        timerClock=SystemCoreClock;
        break;

      case RCC_CFGR_PPRE_DIV4:             // additional multiplier = 2
        timerClock=SystemCoreClock/2;
        break;

      case RCC_CFGR_PPRE_DIV8:             // additional multiplier = 2
        timerClock=SystemCoreClock/4;
        break;

      case RCC_CFGR_PPRE_DIV16:            // additional multiplier = 2
      default:
        timerClock=SystemCoreClock/8;
        break;
    }

    // set up the internal clock

    TIM_InternalClockConfig(timer);
    timer.setClock(timerClock);
  }


  /*
   * Helpers for easy user access to this class
   */

  typedef TimerInternalClockFeature Timer1InternalClockFeature;
  typedef TimerInternalClockFeature Timer2InternalClockFeature;
  typedef TimerInternalClockFeature Timer3InternalClockFeature;
  typedef TimerInternalClockFeature Timer6InternalClockFeature;
  typedef TimerInternalClockFeature Timer14InternalClockFeature;
  typedef TimerInternalClockFeature Timer15InternalClockFeature;
  typedef TimerInternalClockFeature Timer16InternalClockFeature;
  typedef TimerInternalClockFeature Timer17InternalClockFeature;
}
