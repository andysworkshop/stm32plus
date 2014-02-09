/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer6 is a basic timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer6 : public BasicTimer<Timer6<Features...>,PERIPHERAL_TIMER6>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer6<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer6()
        : BasicTimer<TimerType,PERIPHERAL_TIMER6>(TIM6),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
