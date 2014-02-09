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


  /*
   * Remap #1 pin package:
   * (SCL,SDA) = (PF1,PF0)
   */

  struct I2C2Remap1PinPackage {
    enum {
      Port_SCL=GPIOF_BASE,
      Port_SDA=GPIOF_BASE,

      Pin_SCL=GPIO_Pin_1,
      Pin_SDA=GPIO_Pin_0
    };
  };


  /**
   * Class to manage the remap package
   */

  template<class... Features>
  struct I2C2_Remap1 : I2CPeripheral<I2C2Remap1PinPackage,PERIPHERAL_I2C2>,
                       Features... {

    I2C2_Remap1(const Parameters& params)
      : I2CPeripheral<I2C2Remap1PinPackage,PERIPHERAL_I2C2>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };


  /*
   * Remap #2 pin package:
   * (SCL,SDA) = (PH4,PH5)
   */

  struct I2C2Remap2PinPackage {
    enum {
      Port_SCL=GPIOH_BASE,
      Port_SDA=GPIOH_BASE,

      Pin_SCL=GPIO_Pin_4,
      Pin_SDA=GPIO_Pin_5
    };
  };


  /**
   * Class to manage the remap package
   */

  template<class... Features>
  struct I2C2_Remap2 : I2CPeripheral<I2C2Remap2PinPackage,PERIPHERAL_I2C2>,
                       Features... {

    I2C2_Remap2(const Parameters& params)
      : I2CPeripheral<I2C2Remap2PinPackage,PERIPHERAL_I2C2>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };
}
