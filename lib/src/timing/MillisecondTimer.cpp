/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "timing/MillisecondTimer.h"



namespace stm32plus {


  volatile uint32_t MillisecondTimer::_counter;


  /**
   * Initialise SysTick to tick at 1ms by initialising it with SystemCoreClock/1000.
   */

  void MillisecondTimer::initialise() {
    _counter=0;
    SysTick_Config(SystemCoreClock / 1000);
  }


  /**
   * Delay for given time. Waits for the current value of the systick counter to reach a target.
   * @param millis The amount of time to wait.
   */

  void MillisecondTimer::delay(uint32_t millis) {

    uint32_t target;

    target=_counter+millis;
    while(_counter<target);
  }
}


/**
 * SysTick interrupt handler
 */

extern "C" {
  void __attribute__ ((weak,interrupt("IRQ"))) SysTick_Handler(void) {
    stm32plus::MillisecondTimer::_counter++;
  }
}
