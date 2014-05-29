/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to enable OC high level polarity
   */

  struct TimerChannelOCPolarityHighFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCPolarityHighFeature(TimerChannelFeatureBase& tcfb) {
      static_cast<TIM_OCInitTypeDef&>(tcfb).TIM_OCPolarity=TIM_OCPolarity_High;
    }
  };
}
