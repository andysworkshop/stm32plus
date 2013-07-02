/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Common functionality for all timers
   * @param frequency
   */

  class TimerChannelFeatureBase : public TimerFeatureBase {

    public:
      TimerChannelFeatureBase(Timer& timer);

      uint32_t calculateFrequency(uint16_t capture1,uint16_t capture2) const;
  };


  /**
   * Constructor
   * @param timer
   */

  inline TimerChannelFeatureBase::TimerChannelFeatureBase(Timer& timer)
    : TimerFeatureBase(timer) {
  }


  /**
   * Calculate the frequency equivalent to 2 captured values
   * @param capture1
   * @param capture2
   * @return The calculated frequency. Note that it my need to be divided by 2 depending on the value of your APB prescaler.
   */

  inline uint32_t TimerChannelFeatureBase::calculateFrequency(uint16_t capture1,uint16_t capture2) const {

    uint32_t frequency;

    if(capture2>capture1)
      frequency=_timer.getClock()/(capture2-capture1);
    else
      frequency=_timer.getClock()/(65535-capture1+capture2);

    return frequency;
  }
}
