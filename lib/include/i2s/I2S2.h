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
  struct I2S2_Custom : I2SPeripheral<TPinPackage,PERIPHERAL_I2S2>,
                       Features... {

    I2S2_Custom(const typename I2SPeripheral<TPinPackage,PERIPHERAL_I2S2>::Parameters& params)
      : I2SPeripheral<TPinPackage,PERIPHERAL_I2S2>(params),
        Features(static_cast<I2S&>(*this))... {
    }
  };


  /*
   * Default pin package: (WS,CK,SD,MCLK)=(PB12,PB13,PB15,PC6)
   */

  struct I2S2DefaultPinPackage {
    enum {
      Port_WS   = GPIOB_BASE,
      Port_CK   = GPIOB_BASE,
      Port_SD   = GPIOB_BASE,
      Port_MCLK = GPIOC_BASE,

      Pin_WS   = GPIO_Pin_12,
      Pin_CK   = GPIO_Pin_13,
      Pin_SD   = GPIO_Pin_15,
      Pin_MCLK = GPIO_Pin_6
    };
  };


  /**
   * Convenience class to match the F1 pin for pin.
   */

  template<class... Features>
  struct I2S2 : I2SPeripheral<I2S2DefaultPinPackage,PERIPHERAL_I2S2>,
                Features... {

    I2S2(const typename I2SPeripheral<I2S2DefaultPinPackage,PERIPHERAL_I2S2>::Parameters& params)
      : I2SPeripheral<I2S2DefaultPinPackage,PERIPHERAL_I2S2>(params),
        Features(static_cast<I2S&>(*this))... {
    }
  };
}
