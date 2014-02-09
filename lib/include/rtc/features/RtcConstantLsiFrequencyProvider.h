/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief class to return a hard-coded frequency for the LSI. No measuring is done
   *
   * The optimiser will reduce this to a simple load of a constant into a register.
   */

  template<uint32_t TConstantFrequency>
  class RtcConstantLsiFrequencyProvider {

    public:

      static uint32_t getLsiFrequency() {
        return TConstantFrequency;
      }
  };

  /*
   * Since the LSI is supposed to always be 32kHz, provide that as a type
   */

  typedef RtcConstantLsiFrequencyProvider<32768> Rtc32kHzLsiFrequencyProvider;
}
