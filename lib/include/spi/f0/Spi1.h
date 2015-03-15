/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Generic class for any pin package
   */

  template<class TPinPackage,class... Features>
  struct Spi1_Custom : SpiPeripheral<TPinPackage,PERIPHERAL_SPI1>,
                       Features... {

    Spi1_Custom(const typename SpiPeripheral<TPinPackage,PERIPHERAL_SPI1>::Parameters& params)
      : SpiPeripheral<TPinPackage,PERIPHERAL_SPI1>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };


  /*
   * Default pin package:
   */

  struct Spi1DefaultPinPackage {
    enum {
      Port_NSS=GPIOA_BASE,
      Port_SCK=GPIOA_BASE,
      Port_MISO=GPIOA_BASE,
      Port_MOSI=GPIOA_BASE,

      Pin_NSS  = GPIO_Pin_4,
      Pin_SCK  = GPIO_Pin_5,
      Pin_MISO = GPIO_Pin_6,
      Pin_MOSI = GPIO_Pin_7
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Spi1 : SpiPeripheral<Spi1DefaultPinPackage,PERIPHERAL_SPI1>,
                Features... {

    Spi1(const Parameters& params)
      : SpiPeripheral<Spi1DefaultPinPackage,PERIPHERAL_SPI1>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };


  /**
   * NSS is on PA[15]. The others move to PB
   */

  struct Spi1Remap1PinPackage {
    enum {
      Port_NSS=GPIOA_BASE,
      Port_SCK=GPIOB_BASE,
      Port_MISO=GPIOB_BASE,
      Port_MOSI=GPIOB_BASE,

      Pin_NSS  = GPIO_Pin_15,
      Pin_SCK  = GPIO_Pin_3,
      Pin_MISO = GPIO_Pin_4,
      Pin_MOSI = GPIO_Pin_5
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Spi1_Remap1 : SpiPeripheral<Spi1Remap1PinPackage,PERIPHERAL_SPI1>,
                       Features... {

    Spi1_Remap1(const Parameters& params)
      : SpiPeripheral<Spi1Remap1PinPackage,PERIPHERAL_SPI1>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };
}
