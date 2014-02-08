/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * Get the bus speed for SDIO
   */

  template<>
  struct PeripheralBusSpeed<PERIPHERAL_SDIO> {
    static uint32_t getBusSpeed() {
      return SystemCoreClock/RccBusInformation::getAHBPrescaler();
    }
  };
}
