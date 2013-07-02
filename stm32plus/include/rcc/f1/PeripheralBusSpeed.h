/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


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
