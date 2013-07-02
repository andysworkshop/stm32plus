/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer8 is an advanced control timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer8 : public AdvancedControlTimer<Timer8<Features...>,PERIPHERAL_TIMER8>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer8<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer8()
        : AdvancedControlTimer<TimerType,PERIPHERAL_TIMER8>(TIM8),
          Features((Timer&)*this)... {
      }
  };
}
