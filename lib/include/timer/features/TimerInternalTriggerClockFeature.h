/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Feature class to configure the timer to use the update or compare event
   * of another timer as its clock.
   * @tparam TTriggerNumber the internal trigger number (0..3)
   */

  template<uint8_t TTriggerNumber>
  class TimerInternalTriggerClockFeature : public TimerFeatureBase {
    public:
      TimerInternalTriggerClockFeature(Timer& timer);
  };


  /**
   * All four possible specialisations for easy use
   */

  typedef TimerInternalTriggerClockFeature<TIM_TS_ITR0> TimerInternalTrigger0ClockFeature;
  typedef TimerInternalTriggerClockFeature<TIM_TS_ITR1> TimerInternalTrigger1ClockFeature;
  typedef TimerInternalTriggerClockFeature<TIM_TS_ITR2> TimerInternalTrigger2ClockFeature;
  typedef TimerInternalTriggerClockFeature<TIM_TS_ITR3> TimerInternalTrigger3ClockFeature;


  /**
   * Constructor, configure this timer with the trigger number
   */

  template<uint8_t TTriggerNumber>
  inline TimerInternalTriggerClockFeature<TTriggerNumber>::TimerInternalTriggerClockFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_ITRxExternalClockConfig(_timer,TTriggerNumber);
  }
}
