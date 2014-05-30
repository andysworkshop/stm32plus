/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Common functionality for all timer channels
   */

  class TimerChannelFeatureBase : public TimerFeatureBase {

    protected:
      uint8_t _dutyCycle;
      scoped_ptr<TIM_OCInitTypeDef> _oci;

    public:
      TimerChannelFeatureBase(Timer& timer);

      void initialise();

      uint32_t calculateFrequency(uint32_t capture1,uint32_t capture2) const;

      operator TIM_OCInitTypeDef&();
  };


  /**
   * Constructor
   * @param timer
   */

  inline TimerChannelFeatureBase::TimerChannelFeatureBase(Timer& timer)
    : TimerFeatureBase(timer) {
  }


  /**
   * Provide access to the OC structure, creating it where required
   */

  inline TimerChannelFeatureBase::operator TIM_OCInitTypeDef&() {

    // create if it doesn't exist

    if(_oci==nullptr) {
      _oci.reset(new TIM_OCInitTypeDef);
      TIM_OCStructInit(_oci.get());
    }

    return *_oci.get();
  }


  /**
   * Calculate the frequency equivalent to 2 captured values
   * @param capture1
   * @param capture2
   * @return The calculated frequency. Note that it my need to be divided by 2 depending on the value of your APB prescaler.
   */

  inline uint32_t TimerChannelFeatureBase::calculateFrequency(uint32_t capture1,uint32_t capture2) const {

    uint32_t frequency;

    if(capture2>capture1)
      frequency=_timer.getClock()/(capture2-capture1);
    else
      frequency=_timer.getClock()/(_timer.getCounterMax()-capture1+capture2);

    return frequency;
  }
}
