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
   * Specialisation of the AF mapper for this peripheral
   */

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_SPI1,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_SPI1
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_SPI2,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_SPI2
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_SPI3,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_SPI3
    };
  };
}
