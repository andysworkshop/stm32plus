/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer14 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer14 : public GeneralPurposeTimer<Timer14<Features...>,PERIPHERAL_TIMER14>,
                  public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer14<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer14()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER14>(TIM14),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
