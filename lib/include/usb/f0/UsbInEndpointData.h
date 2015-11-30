/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0_42
#error This class can only be used with the STM32F042 series
#endif


namespace stm32plus {

  /*
   * Simple struct to manage pointer and length of the remaining data to be sent
   * to an IN endpoint
   */

  struct UsbInEndpointData {
    const uint8_t *ptr;
    uint16_t remaining;
    uint16_t total;
    uint16_t maxPacketSize;
    uint16_t pmaAddress;

    /*
     * Zero out the contents on construction
     */

    UsbInEndpointData()
      : ptr(nullptr),
        remaining(0) {
    }
  };
}
