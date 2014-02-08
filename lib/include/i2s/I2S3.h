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
  struct I2S3_Custom : I2SPeripheral<TPinPackage,PERIPHERAL_I2S3>,
                       Features... {

    I2S3_Custom(const typename I2SPeripheral<TPinPackage,PERIPHERAL_I2S3>::Parameters& params)
      : I2SPeripheral<TPinPackage,PERIPHERAL_I2S3>(params),
        Features(static_cast<I2S&>(*this))... {
    }
  };


  /*
   * Default pin package: (WS,CK,SD,MCLK)=(PA15,PB3,PB5,PC7)
   */

  struct I2S3DefaultPinPackage {
    enum {
      Port_WS   = GPIOA_BASE,
      Port_CK   = GPIOB_BASE,
      Port_SD   = GPIOB_BASE,
      Port_MCLK = GPIOC_BASE,

      Pin_WS   = GPIO_Pin_15,
      Pin_CK   = GPIO_Pin_3,
      Pin_SD   = GPIO_Pin_5,
      Pin_MCLK = GPIO_Pin_7
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct I2S3 : I2SPeripheral<I2S3DefaultPinPackage,PERIPHERAL_I2S3>,
                Features... {

    I2S3(const Parameters& params)
      : I2SPeripheral<I2S3DefaultPinPackage,PERIPHERAL_I2S3>(params),
        Features(static_cast<I2S&>(*this))... {
    }
  };
}

