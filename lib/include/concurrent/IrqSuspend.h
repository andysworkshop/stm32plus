/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Simple utility structure to enable temporary suspension of IRQs on construction and
   * re-enable on destruction. This is used quite heavily in the net code where there are
   * shared data structures between IRQ servicing and regular code.
   */

  struct IrqSuspend {

    /**
     * The counter
     */

    static uint32_t _counter;


    /**
     * Constructor
     */

    IrqSuspend() {
      suspend();
    }


    /**
     * Destructor
     */

    ~IrqSuspend() {
      resume();
    }


    /**
     * Suspend IRQs if they're not already suspended. We maintain an atomic counter
     * that ensures we can nest calls to suspend() - resume() pairs.
     */

    static void suspend() {

      if(sync_fetch_and_increment(&_counter)==0)
        Nvic::disableAllInterrupts();
    }


    /**
     * Decrement the counter and re-enable interrupts if we are the last in the nest
     * of callers.
     */

    static void resume() {
      if(sync_decrement_and_fetch(&_counter)==0)
        Nvic::enableAllInterrupts();
    }
  };
}
