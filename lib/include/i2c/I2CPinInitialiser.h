/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Convenience template to initialise an I2C pin package
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  class I2CPinInitialiser {
    public:
      static void initialise();
  };


  /**
   * Initialise the pin package that we were parameterised with. The pin modes are the
   * same on the F0, F1 and F4 - both are AF-OD.
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline void I2CPinInitialiser<TPinPackage,TPeripheralName>::initialise() {

    GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_SCL,
                                   TPinPackage::Pin_SCL,
                                   Gpio::ALTERNATE_FUNCTION,
                                   (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                   Gpio::PUPD_NONE,
                                   Gpio::OPEN_DRAIN,
                                   GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_SCL,TPinPackage::Pin_SCL>::GPIO_AF);

    GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_SDA,
                                   TPinPackage::Pin_SDA,
                                   Gpio::ALTERNATE_FUNCTION,
                                   (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                   Gpio::PUPD_NONE,
                                   Gpio::OPEN_DRAIN,
                                   GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_SDA,TPinPackage::Pin_SDA>::GPIO_AF);
  }
}
