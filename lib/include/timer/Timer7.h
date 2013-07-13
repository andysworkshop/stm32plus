/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer7 is a basic timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer7 : public BasicTimer<Timer7<Features...>,PERIPHERAL_TIMER7>,
                 public Features... {

    public:

  		/**
			 * Helper typedef to get at this type
			 */

      typedef Timer7<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer7()
        : BasicTimer<TimerType,PERIPHERAL_TIMER7>(TIM7),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
