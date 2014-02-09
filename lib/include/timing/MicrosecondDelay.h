/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Delay for a number of microseconds.
   * We use an STM32 timer to provide a microsecond delay. We're agnostic as to which timer you use
   * and this template is parameterised by that timer class. A concrete type 'MicrosecondDelay' is typedef'd
   * that uses TIM6 as the timer.
   * @tparam TTimer The timer type to use for this functionality.
   */

  template<class TTimer>
  class MicrosecondDelayTemplate {

    protected:
      static TTimer *_timer;
      static volatile uint16_t *_counterRegister;

    public:

      /**
       * Initialise the timer.
       */

      static void initialise();

      /**
       * Delay synchronously to the caller for the given number of microseconds.
       * @param[in] us The number of microseconds to delay for.
       */

      static void delay(uint16_t us);
  };


  /**
   * Use the basic timer TIM6 as a default for doing the microsecond delay.
   */

  typedef MicrosecondDelayTemplate<Timer6<Timer6InternalClockFeature> > MicrosecondDelay;


  /**
   * Template static member initialisation
   */

  template<class TTimer> TTimer * MicrosecondDelayTemplate<TTimer>::_timer;
  template<class TTimer> volatile uint16_t *MicrosecondDelayTemplate<TTimer>::_counterRegister;


  /**
   * Initialise the timer by setting it up to tick at 1Mhz
   */

  template<class TTimer>
  inline void MicrosecondDelayTemplate<TTimer>::initialise() {

    uint32_t regbase;

    // create the timer object and configure the timebase

    _timer=new TTimer;
    _timer->setTimeBaseByFrequency(1000000,UINT16_MAX);

    // enable the timer

    _timer->enablePeripheral();

    // remember the counter register

    regbase=(uint32_t)((TIM_TypeDef *)*_timer);
    _counterRegister=(volatile uint16_t *)(regbase+__builtin_offsetof(TIM_TypeDef,CNT));
  }


  /**
   * Delay for the given microseconds. The total delay is approximate. There is a clock-speed
   * dependent constant overhead to calling into, and returning from, this function. Attempting to take
   * this call delay into account without coding the whole thing in asm is folly - the optimiser and
   * the CPU instruction pipeline will all conspire to ensure you get it wrong.
   * @param microseconds the number of microseconds to delay for.
   */

  template<class TTimer>
  inline void MicrosecondDelayTemplate<TTimer>::delay(uint16_t microseconds) {

    // reset the counter register to zero then wait for it to
    // tick up by the required number of microseconds

    *_counterRegister=0;
    while(*_counterRegister<=microseconds);
  }
}
