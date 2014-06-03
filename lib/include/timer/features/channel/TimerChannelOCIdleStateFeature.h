/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the OC idle state
   */

  template<uint16_t TIdleState>
  struct TimerChannelOCIdleStateFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCIdleStateFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TIdleState==TIM_OCIdleState_Set || TIdleState==TIM_OCIdleState_Reset,"Invalid idle state type");
      static_cast<TIM_OCInitTypeDef&>(tcfb).TIM_OCIdleState=TIdleState;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelOCIdleStateFeature<TIM_OCIdleState_Set> TimerChannelOCHighIdleFeature;
  typedef TimerChannelOCIdleStateFeature<TIM_OCIdleState_Reset> TimerChannelOCLowIdleFeature;
}
