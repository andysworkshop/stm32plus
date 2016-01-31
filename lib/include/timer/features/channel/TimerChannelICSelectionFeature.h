/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Feature class to set the IC selection (internal connections)
   */

  template<uint16_t TSelection>
  struct TimerChannelICSelectionFeature {

    /**
     * Constructor, set the flag in the IC struct
     */

    TimerChannelICSelectionFeature(TimerChannelFeatureBase& tcfb) {
      static_assert(TSelection==TIM_ICSelection_DirectTI ||
                    TSelection==TIM_ICSelection_IndirectTI ||
                    TSelection==TIM_ICSelection_TRC,"Invalid IC selection type");

      TIM_ICInitTypeDef& ici=static_cast<TIM_ICInitTypeDef&>(tcfb);

      ici.TIM_ICSelection=TSelection;
    }
  };

  /**
   * Typedefs that avoid the templates
   */

  typedef TimerChannelICSelectionFeature<TIM_ICSelection_DirectTI> TimerChannelICDirectTiFeature;
  typedef TimerChannelICSelectionFeature<TIM_ICSelection_IndirectTI> TimerChanneICIndirectTiFeature;
  typedef TimerChannelICSelectionFeature<TIM_ICSelection_TRC> TimerChannelICTrcFeature;
}
