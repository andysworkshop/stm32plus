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
   * 8 byte header sent in the SETUP request
   */

  struct UsbSetupHeader {

    uint8_t bmRequest;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;

    UsbSetupHeader(const void *vptr) {

      const uint8_t *ptr;

      ptr=reinterpret_cast<const uint8_t *>(vptr);

      bmRequest=ptr[0];
      bRequest=ptr[1];
      wValue=*reinterpret_cast<const uint16_t *>(ptr+2);
      wIndex=*reinterpret_cast<const uint16_t *>(ptr+4);
      wLength=*reinterpret_cast<const uint16_t *>(ptr+6);
    }
  };
}
