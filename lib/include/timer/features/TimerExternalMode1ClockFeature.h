/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * feature class to configure the timer as using an external clock as
   * a trigger. The counter can count at each rising or falling edge on
   * a selected input.
   *
   * The user must call setClock() on the the timer object.
   *
   * @tparam TTriggerSource TIM_TS_ITR0..3
   */

  template<uint16_t TTriggerSource>
  class TimerExternalMode1ClockFeature : public TimerFeatureBase {
    public:
      TimerExternalMode1ClockFeature(Timer& timer);
  };


  /**
   * Constructor. Calls TIM_ITRxExternalClockConfig for this timer
   */

  template<uint16_t TTriggerSource>
  inline TimerExternalMode1ClockFeature<TTriggerSource>::TimerExternalMode1ClockFeature(Timer& timer)
    : TimerFeatureBase(timer) {
    TIM_ITRxExternalClockConfig(timer,TTriggerSource);
  }
}
