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
  struct I2C3_Custom : I2CPeripheral<TPinPackage,PERIPHERAL_I2C3>,
                       Features... {

    I2C3_Custom(const typename I2CPeripheral<TPinPackage,PERIPHERAL_I2C3>::Parameters& params)
      : I2CPeripheral<TPinPackage,PERIPHERAL_I2C3>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };


  /*
   * Default pin package:
   * (SCL,SDA) = (PH7,PH8)
   */

  struct I2C3DefaultPinPackage {
    enum {
      Port_SCL=GPIOH_BASE,
      Port_SDA=GPIOH_BASE,

      Pin_SCL=GPIO_Pin_7,
      Pin_SDA=GPIO_Pin_8
    };
  };


  /**
   * This peripheral is unique to the F4. We'll call this one the default pin set.
   */

  template<class... Features>
  struct I2C3_Default : I2CPeripheral<I2C3DefaultPinPackage,PERIPHERAL_I2C3>,
                        Features... {

    I2C3_Default(const Parameters& params)
      : I2CPeripheral<I2C3DefaultPinPackage,PERIPHERAL_I2C3>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };


  /*
   * Remap #1:
   * (SCL,SDA) = (PA8,PC9)
   */

  struct I2C3RemapPinPackage {
    enum {
      Port_SCL=GPIOA_BASE,
      Port_SDA=GPIOC_BASE,

      Pin_SCL=GPIO_Pin_8,
      Pin_SDA=GPIO_Pin_9
    };
  };


  /**
   * Remap class
   */

  template<class... Features>
  struct I2C3_Remap : I2CPeripheral<I2C3RemapPinPackage,PERIPHERAL_I2C3>,
                      Features... {

    I2C3_Remap(const Parameters& params)
      : I2CPeripheral<I2C3RemapPinPackage,PERIPHERAL_I2C3>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };
}
