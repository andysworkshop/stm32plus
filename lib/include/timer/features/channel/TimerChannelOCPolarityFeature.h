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

  template<uint16_t TPolarity>
  struct TimerChannelOCPolarityFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCPolarityFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TPolarity==TIM_OCPolarity_High || TPolarity==TIM_OCPolarity_Low,"Invalid polarity type");

      TIM_OCInitTypeDef& oci=static_cast<TIM_OCInitTypeDef&>(tcfb);

      oci.TIM_OCPolarity=TPolarity;
      oci.TIM_OutputState=TIM_OutputState_Enable;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelOCPolarityFeature<TIM_OCPolarity_High> TimerChannelOCHighPolarityFeature;
  typedef TimerChannelOCPolarityFeature<TIM_OCPolarity_Low> TimerChannelOCLowPolarityFeature;
}
