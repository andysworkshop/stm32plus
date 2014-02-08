/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Generic class for any pin package
   */

  template<class TPinPackage,class... Features>
  struct Spi2_Custom : SpiPeripheral<TPinPackage,PERIPHERAL_SPI2>,
                       Features... {

    Spi2_Custom(const typename SpiPeripheral<TPinPackage,PERIPHERAL_SPI2>::Parameters& params)
      : SpiPeripheral<TPinPackage,PERIPHERAL_SPI2>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };


  /*
   * Default pin package:
   */

  struct Spi2DefaultPinPackage {
    enum {
      Port_NSS=GPIOB_BASE,
      Port_SCK=GPIOB_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_12,
      Pin_SCK  = GPIO_Pin_13,
      Pin_MISO = GPIO_Pin_14,
      Pin_MOSI = GPIO_Pin_15
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Spi2 : SpiPeripheral<Spi2DefaultPinPackage,PERIPHERAL_SPI2>,
                Features... {

    Spi2(const Parameters& params)
      : SpiPeripheral<Spi2DefaultPinPackage,PERIPHERAL_SPI2>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };
}
