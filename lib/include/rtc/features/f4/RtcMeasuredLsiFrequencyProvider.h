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
   * Class to measure the frequency of the LSI using its output into channel 4
   * of Timer5. Of course this method fundamentally relies on the accuracy of the
   * oscillator (probably HSE) being better than that of the LSI itself.
   *
   * As an observation, the LSI on my STM32F4DISCOVERY board was measured at about 37.4kHz,
   * way off the target of 32.768kHz. This led to the clock running about 5 seconds fast per
   * minute. Using this class as the provider of the LSI frequency successfully calibrated
   * the LSI and got me an accurate RTC 1Hz tick.
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
