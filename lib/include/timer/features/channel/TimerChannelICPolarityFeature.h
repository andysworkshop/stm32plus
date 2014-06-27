/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the IC polarity
   */

  template<uint16_t TPolarity>
  struct TimerChannelICPolarityFeature {

    /**
     * Constructor, set the flag in the IC struct
     */

    TimerChannelICPolarityFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TPolarity==TIM_ICPolarity_Rising || TPolarity==TIM_ICPolarity_Falling || TPolarity==TIM_ICPolarity_BothEdge,"Invalid polarity type");

      TIM_ICInitTypeDef& ici=static_cast<TIM_ICInitTypeDef&>(tcfb);

      ici.TIM_ICPolarity=TPolarity;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelICPolarityFeature<TIM_ICPolarity_Rising> TimerChannelICRisingEdgeFeature;
  typedef TimerChannelICPolarityFeature<TIM_ICPolarity_Falling> TimerChanneICFallingEdgeFeature;
  typedef TimerChannelICPolarityFeature<TIM_ICPolarity_BothEdge> TimerChannelICBothEdgesFeature;
}
