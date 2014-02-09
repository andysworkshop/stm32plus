/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


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
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
