/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer2 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer2 : public GeneralPurposeTimer<Timer2<Features...>,PERIPHERAL_TIMER2>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer2<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer2()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER2>(TIM2),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
