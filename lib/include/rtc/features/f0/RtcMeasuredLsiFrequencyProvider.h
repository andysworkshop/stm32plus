/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Class to measure the frequency of the LSI using its output into channel 1
   * of Timer14. Of course this method fundamentally relies on the accuracy of the
   * oscillator that runs the timer (e.g. HSI) being better than that of the LSI itself.
   */

  class RtcMeasuredLsiFrequencyProvider {

    /**
     * Compute the LSI frequency
     * @return The measured frequency
     */

    public:
      static uint32_t getLsiFrequency();
  };
}
