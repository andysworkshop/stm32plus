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
   * Generic class for any pin package
   */

  template<class TPinPackage,class... Features>
  struct I2C1_Custom : I2CPeripheral<TPinPackage,PERIPHERAL_I2C1>,
                       Features... {

    I2C1_Custom(const typename I2CPeripheral<TPinPackage,PERIPHERAL_I2C1>::Parameters& params)
      : I2CPeripheral<TPinPackage,PERIPHERAL_I2C1>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };


  /*
   * Default pin package:
   * (SCL,SDA) = (PB6,PB7)
   */

  struct I2C1DefaultPinPackage {
    enum {
      Port_SCL=GPIOB_BASE,
      Port_SDA=GPIOB_BASE,

      Pin_SCL=GPIO_Pin_6,
      Pin_SDA=GPIO_Pin_7
    };
  };


  /**
   * Convenience class to match the F1 pin for pin. This can't be named I2C1 because of
   * a name clash in the std peripheral library
   */

  template<class... Features>
  struct I2C1_Default : I2CPeripheral<I2C1DefaultPinPackage,PERIPHERAL_I2C1>,
                        Features... {

    I2C1_Default(const Parameters& params)
      : I2CPeripheral<I2C1DefaultPinPackage,PERIPHERAL_I2C1>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };


  /*
   * Remap #1:
   * (SCL,SDA) = (PB8,PB9)
   */

  struct I2C1RemapPinPackage {
    enum {
      Port_SCL=GPIOB_BASE,
      Port_SDA=GPIOB_BASE,

      Pin_SCL=GPIO_Pin_8,
      Pin_SDA=GPIO_Pin_9
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct I2C1_Remap : I2CPeripheral<I2C1RemapPinPackage,PERIPHERAL_I2C1>,
                      Features... {

    I2C1_Remap(const Parameters& params)
      : I2CPeripheral<I2C1RemapPinPackage,PERIPHERAL_I2C1>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };
}
