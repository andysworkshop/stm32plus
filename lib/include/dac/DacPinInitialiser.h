/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Convenience template to initialise a DAC pin package
   */

  template<class TPinPackage>
  class DacPinInitialiser {
    public:
      static void initialise();
  };


  /**
   * Initialise the pin package that we were parameterised with
   */

  template<class TPinPackage>
  inline void DacPinInitialiser<TPinPackage>::initialise() {

    GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_OUT,
                                   TPinPackage::Pin_OUT,
                                   Gpio::ANALOG,
                                   GPIO_Speed_50MHz,        // unused
                                   Gpio::PUPD_NONE,
                                   Gpio::PUSH_PULL);
  }
}
