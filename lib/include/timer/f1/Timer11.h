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
   * Timer11 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer11 : public GeneralPurposeTimer<Timer11<Features...>,PERIPHERAL_TIMER11>,
                  public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer11<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer11()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER11>(TIM11),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
