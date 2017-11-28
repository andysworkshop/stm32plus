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

  /**
   * USB configuration flags that go into the bmAttributes field of
   * the configuration descriptor
   */

  enum UsbConfigurationFlags {
    REMOTE_WAKEUP = 0x20,
    SELF_POWERED = 0x40
  };
}
