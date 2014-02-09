/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Feature class to configure the timer as using the internal clock as
   * its source.
   */

  template<uint8_t TAPBBusNumber>
  class TimerInternalClockFeature : public TimerFeatureBase {
    public:
      TimerInternalClockFeature(Timer& timer);
  };


  /**
   * Specialisations for APB1 and APB2. The bus prescaler and the not very well documented
   * x2 multiplier that applies when the prescaler is not 1 are taken into account when
   * setting the timer clock.
   *
   * TIM1,8,9,10,11 are on APB2. The remainder are on APB1
   */

  template<>
  inline TimerInternalClockFeature<1>::TimerInternalClockFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    uint32_t timerClock;

    switch(RCC->CFGR & RCC_CFGR_PPRE1) {

      case RCC_CFGR_PPRE1_DIV1:             // additional multiplier = 1
      case RCC_CFGR_PPRE1_DIV2:             // additional multiplier = 2
        timerClock=SystemCoreClock;
        break;

      case RCC_CFGR_PPRE1_DIV4:             // additional multiplier = 2
        timerClock=SystemCoreClock/2;
        break;

      case RCC_CFGR_PPRE1_DIV8:             // additional multiplier = 2
        timerClock=SystemCoreClock/4;
        break;

      case RCC_CFGR_PPRE1_DIV16:            // additional multiplier = 2
      default:
        timerClock=SystemCoreClock/8;
        break;
    }

    // set up the internal clock

    TIM_InternalClockConfig(timer);
    timer.setClock(timerClock);
  }


  template<>
  inline TimerInternalClockFeature<2>::TimerInternalClockFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    uint32_t timerClock;

    switch(RCC->CFGR & RCC_CFGR_PPRE2) {

      case RCC_CFGR_PPRE2_DIV1:             // additional multiplier = 1
      case RCC_CFGR_PPRE2_DIV2:             // additional multiplier = 2
        timerClock=SystemCoreClock;
        break;

      case RCC_CFGR_PPRE2_DIV4:             // additional multiplier = 2
        timerClock=SystemCoreClock/2;
        break;

      case RCC_CFGR_PPRE2_DIV8:             // additional multiplier = 2
        timerClock=SystemCoreClock/4;
        break;

      case RCC_CFGR_PPRE2_DIV16:            // additional multiplier = 2
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

  typedef TimerInternalClockFeature<2> Timer1InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer2InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer3InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer4InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer5InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer6InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer7InternalClockFeature;
  typedef TimerInternalClockFeature<2> Timer8InternalClockFeature;
  typedef TimerInternalClockFeature<2> Timer9InternalClockFeature;
  typedef TimerInternalClockFeature<2> Timer10InternalClockFeature;
  typedef TimerInternalClockFeature<2> Timer11InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer12InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer13InternalClockFeature;
  typedef TimerInternalClockFeature<1> Timer14InternalClockFeature;
}
