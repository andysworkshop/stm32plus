/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the OCN polarity
   */

  template<uint16_t TPolarity>
  struct TimerChannelOCNPolarityFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCNPolarityFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TPolarity==TIM_OCNPolarity_High || TPolarity==TIM_OCNPolarity_Low,"Invalid polarity type");

      TIM_OCInitTypeDef& oci=static_cast<TIM_OCInitTypeDef&>(tcfb);

      oci.TIM_OCNPolarity=TPolarity;
      oci.TIM_OutputNState=TIM_OutputNState_Enable;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelOCNPolarityFeature<TIM_OCNPolarity_High> TimerChannelOCNHighPolarityFeature;
  typedef TimerChannelOCNPolarityFeature<TIM_OCNPolarity_Low> TimerChannelOCNLowPolarityFeature;
}
