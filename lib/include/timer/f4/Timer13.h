/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Timer13 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer13 : public GeneralPurposeTimer<Timer13<Features...>,PERIPHERAL_TIMER13>,
                  public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer13<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer13()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER13>(TIM13),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
