/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the OCN idle state
   */

  template<uint16_t TIdleState>
  struct TimerChannelOCNIdleStateFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCNIdleStateFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TIdleState==TIM_OCNIdleState_Set || TIdleState==TIM_OCNIdleState_Reset,"Invalid N idle state type");
      static_cast<TIM_OCInitTypeDef&>(tcfb).TIM_OCNIdleState=TIdleState;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelOCNIdleStateFeature<TIM_OCNIdleState_Set> TimerChannelOCNHighIdleFeature;
  typedef TimerChannelOCNIdleStateFeature<TIM_OCNIdleState_Reset> TimerChannelOCNLowIdleFeature;
}
