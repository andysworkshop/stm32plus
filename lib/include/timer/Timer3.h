/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer3 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer3 : public GeneralPurposeTimer<Timer3<Features...>,PERIPHERAL_TIMER3>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer3<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer3()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER3>(TIM3),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
