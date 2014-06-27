/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the IC filter to a value between 0 and 0xf
   */

  template<uint8_t TFilter>
  struct TimerChannelICFilterFeature {

    /**
     * Constructor, set the value in the IC struct
     */

    TimerChannelICFilterFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TFilter<=0xf,"Invalid IC filter value");

      TIM_ICInitTypeDef& ici=static_cast<TIM_ICInitTypeDef&>(tcfb);

      ici.TIM_ICFilter=TFilter;
    }
  };
}
