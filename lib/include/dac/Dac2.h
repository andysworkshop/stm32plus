/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /*
   * Default pin package: (OUT = PA5)
   */

  struct Dac2DefaultPinPackage {
    enum {
      Port_OUT = GPIOA_BASE,
      Pin_OUT  = GPIO_Pin_5
    };
  };


  /**
   * Main DAC class
   */

  template<class... Features>
  struct Dac2 : DacPeripheral<Dac2DefaultPinPackage,PERIPHERAL_DAC2>,
                Features... {

    Dac2(const Parameters& params=Parameters())
      : DacPeripheral<Dac2DefaultPinPackage,PERIPHERAL_DAC2>(params),
        Features((Dac&)*this)... {
    }
  };
}
