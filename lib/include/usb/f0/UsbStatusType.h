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
   * Status constants sent by the UsbStatus event
   */

  enum class UsbStatusType {
    RESET,
    PACKET_MEMORY_OVER_UNDERFLOW,
    ERROR,
    START_OF_FRAME,
    UNEXPECTED_START_OF_FRAME,
    STATE_DEFAULT,
    STATE_ADDRESSED,
    STATE_CONFIGURED,
    STATE_SUSPENDED
  };
}
