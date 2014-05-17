/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * Default pin package: (OUT = PA4)
   */

  struct Dac1DefaultPinPackage {
    enum {
      Port_OUT = GPIOA_BASE,
      Pin_OUT  = GPIO_Pin_4
    };
  };


  /**
   * Main DAC class
   */

  template<class... Features>
  struct Dac1 : DacPeripheral<Dac1DefaultPinPackage,PERIPHERAL_DAC1>,
                Features... {

    Dac1(const Parameters& params=Parameters())
      : DacPeripheral<Dac1DefaultPinPackage,PERIPHERAL_DAC1>(params),
        Features((Dac&)*this)... {
    }
  };
}
