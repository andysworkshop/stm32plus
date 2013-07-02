/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer5 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer5 : public GeneralPurposeTimer<Timer5<Features...>,PERIPHERAL_TIMER5>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer5<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer5()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER5>(TIM5),
          Features((Timer&)*this)... {
      }
  };
}
