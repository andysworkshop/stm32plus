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
   * Timer4 is a general purpose timer. The template parameters can be used to specify a list
   * of timer features that you want to use.
   */

  template<class... Features>
  class Timer4 : public GeneralPurposeTimer<Timer4<Features...>,PERIPHERAL_TIMER4>,
                 public Features... {

    public:

      /**
       * Helper typedef to get at this type
       */

      typedef Timer4<Features...> TimerType;

      /**
       * Constructor. Start the clocks
       */

      Timer4()
        : GeneralPurposeTimer<TimerType,PERIPHERAL_TIMER4>(TIM4),
          Features(static_cast<Timer&>(*this))... {
      }
  };
}
