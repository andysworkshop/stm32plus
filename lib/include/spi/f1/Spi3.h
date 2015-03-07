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
   * Generic class for any pin package
   */

  template<class TPinPackage,class... Features>
  struct Spi3_Custom : SpiPeripheral<TPinPackage,PERIPHERAL_SPI3>,
                       Features... {

    Spi3_Custom(const typename SpiPeripheral<TPinPackage,PERIPHERAL_SPI3>::Parameters& params)
      : SpiPeripheral<TPinPackage,PERIPHERAL_SPI3>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };


  /*
   * Default pin package:
   */

  struct Spi3DefaultPinPackage {
    enum {
      Port_NSS =GPIOA_BASE,
      Port_SCK =GPIOB_BASE,
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
  struct Spi3 : SpiPeripheral<Spi3DefaultPinPackage,PERIPHERAL_SPI3>,
                Features... {

    Spi3(const Parameters& params)
      : SpiPeripheral<Spi3DefaultPinPackage,PERIPHERAL_SPI3>(params),
        Features(static_cast<Spi&>(*this))... {
    }
  };

#if defined(STM32PLUS_F1_CL)

  /**
   * NSS is on PA[15]. The others move to PB
   */

  struct Spi3Remap1PinPackage {
    enum {
      Port_NSS=GPIOA_BASE,
      Port_SCK=GPIOC_BASE,
      Port_MISO=GPIOC_BASE,
      Port_MOSI=GPIOC_BASE,

      Pin_NSS  = GPIO_Pin_4,
      Pin_SCK  = GPIO_Pin_10,
      Pin_MISO = GPIO_Pin_11,
      Pin_MOSI = GPIO_Pin_12
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct Spi3_Remap1 : SpiPeripheral<Spi3Remap1PinPackage,PERIPHERAL_SPI3>,
                       Features... {

    Spi3_Remap1(const Parameters& params)
      : SpiPeripheral<Spi3Remap1PinPackage,PERIPHERAL_SPI3>(params),
        Features(static_cast<Spi&>(*this))... {

      GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
    }
  };
#endif
}

