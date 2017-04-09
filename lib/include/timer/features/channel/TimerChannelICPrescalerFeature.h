/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the IC prescaler
   */

  template<uint16_t TPrescaler>
  struct TimerChannelICPrescalerFeature {

    /**
     * Constructor, set the flag in the IC struct
     */

    TimerChannelICPrescalerFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TPrescaler==TIM_ICPSC_DIV1 ||
                    TPrescaler==TIM_ICPSC_DIV2 ||
                    TPrescaler==TIM_ICPSC_DIV4 ||
                    TPrescaler==TIM_ICPSC_DIV8,"Invalid IC prescaler");

      TIM_ICInitTypeDef& ici=static_cast<TIM_ICInitTypeDef&>(tcfb);

      ici.TIM_ICPrescaler=TPrescaler;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelICPrescalerFeature<TIM_ICPSC_DIV1> TimerChannelICPreScaler1Feature;
  typedef TimerChannelICPrescalerFeature<TIM_ICPSC_DIV2> TimerChannelICPreScaler2Feature;
  typedef TimerChannelICPrescalerFeature<TIM_ICPSC_DIV4> TimerChannelICPreScaler4Feature;
  typedef TimerChannelICPrescalerFeature<TIM_ICPSC_DIV8> TimerChannelICPreScaler8Feature;
}
