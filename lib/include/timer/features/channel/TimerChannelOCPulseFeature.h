/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the OC polarity
   */

  template<uint16_t TPulse>
  struct TimerChannelOCPulseFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCPulseFeature(TimerChannelFeatureBase& tcfb) {
      static_cast<TIM_OCInitTypeDef&>(tcfb).TIM_Pulse=TPulse;
    }
  };
}
