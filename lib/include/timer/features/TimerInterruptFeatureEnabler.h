/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each timer
   * @tparam TTimerNumber The number of the timer peripheral
   */

  template<uint8_t TTimerNumber>
  class TimerInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR 
      static FPTR _forceLinkage;

    public:
      static void enable(uint16_t interruptMask,uint8_t priority,uint8_t subPriority=0);
  };
  
}
