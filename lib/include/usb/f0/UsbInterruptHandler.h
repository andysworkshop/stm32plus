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
   * USB interrupt event handling
   */

  DECLARE_EVENT_SIGNATURE(UsbInterrupt,void());

  struct UsbInterruptHandler {
    DECLARE_EVENT_SOURCE(UsbInterrupt);
  };

  // declare reference to the one and only instance of this structure

  extern UsbInterruptHandler usbInterrupt;
}
