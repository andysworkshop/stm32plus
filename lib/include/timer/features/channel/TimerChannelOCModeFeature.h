/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to enable OC mode
   */

  template<uint16_t TMode>
  struct TimerChannelOCModeFeature {

    /**
     * Constructor, set the flag in the OC struct
     */

    TimerChannelOCModeFeature(TimerChannelFeatureBase& tcfb) {

      static_assert(TMode==TIM_OCMode_Timing ||
                    TMode==TIM_OCMode_Active ||
                    TMode==TIM_OCMode_Inactive ||
                    TMode==TIM_OCMode_Toggle ||
                    TMode==TIM_OCMode_PWM1 ||
                    TMode==TIM_OCMode_PWM2,"Invalid OC mode");

      static_cast<TIM_OCInitTypeDef&>(tcfb).TIM_OCMode=TMode;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelOCModeFeature<TIM_OCMode_Timing> TimerChannelOCTimingModeFeature;
  typedef TimerChannelOCModeFeature<TIM_OCMode_Active> TimerChannelOCActiveModeFeature;
  typedef TimerChannelOCModeFeature<TIM_OCMode_Inactive> TimerChannelOCInactiveModeFeature;
  typedef TimerChannelOCModeFeature<TIM_OCMode_Toggle> TimerChannelOCToggleModeFeature;
  typedef TimerChannelOCModeFeature<TIM_OCMode_PWM1> TimerChannelOCPWM1ModeFeature;
  typedef TimerChannelOCModeFeature<TIM_OCMode_PWM2> TimerChannelOCPWM2ModeFeature;
}
