/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#if !defined(STM32PLUS_F1)
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * Convenience template to initialise a CAN pin package
   */

  template<class TPinPackage,PeripheralName>
  class CanPinInitialiser {
    public:
      static void initialise();
  };


  /**
   * Initialise the pin package that we were parameterised with
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline void CanPinInitialiser<TPinPackage,TPeripheralName>::initialise() {

    GpioPinInitialiser::initialise(
        (GPIO_TypeDef *)TPinPackage::Port_TX,
        TPinPackage::Pin_TX,
        Gpio::ALTERNATE_FUNCTION,
        (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
        Gpio::PUPD_NONE,
        Gpio::PUSH_PULL,
        GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_TX,TPinPackage::Pin_TX>::GPIO_AF);

    GpioPinInitialiser::initialise(
        (GPIO_TypeDef *)TPinPackage::Port_RX,
        TPinPackage::Pin_RX,
				Gpio::INPUT,
				(GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
				Gpio::PUPD_UP,
				GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_RX,TPinPackage::Pin_RX>::GPIO_AF);
	}
}
