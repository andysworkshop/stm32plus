/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer1 is an advanced control timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer1 : public AdvancedControlTimer<Timer1<Features...>,PERIPHERAL_TIMER1>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer1<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer1()
        : AdvancedControlTimer<TimerType,PERIPHERAL_TIMER1>(TIM1),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
