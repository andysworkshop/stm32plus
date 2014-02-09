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
  struct I2C2_Custom : I2CPeripheral<TPinPackage,PERIPHERAL_I2C2>,
                       Features... {

    I2C2_Custom(const typename I2CPeripheral<TPinPackage,PERIPHERAL_I2C2>::Parameters& params)
      : I2CPeripheral<TPinPackage,PERIPHERAL_I2C2>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };


  /*
   * Default pin package:
   * (SCL,SDA) = (PB10,PB11)
   */

  struct I2C2DefaultPinPackage {
    enum {
      Port_SCL=GPIOB_BASE,
      Port_SDA=GPIOB_BASE,

      Pin_SCL=GPIO_Pin_10,
      Pin_SDA=GPIO_Pin_11
    };
  };


  /**
   * Convenience class to match the F1 pin for pin. This can't be named I2C2 because of
   * a name clash in the std peripheral library
   */

  template<class... Features>
  struct I2C2_Default : I2CPeripheral<I2C2DefaultPinPackage,PERIPHERAL_I2C2>,
                        Features... {

    I2C2_Default(const Parameters& params)
      : I2CPeripheral<I2C2DefaultPinPackage,PERIPHERAL_I2C2>(params),
        Features(static_cast<I2C&>(*this))... {
    }
  };
}
