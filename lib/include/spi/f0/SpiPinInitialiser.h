/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Convenience template to initialise an SPI pin package
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  class SpiPinInitialiser {
    public:
      static void initialise(uint16_t mode,uint16_t direction);
  };


  /**
   * Initialise the pin package that we were parameterised with
   */

  template<class TPinPackage,PeripheralName TPeripheralName>
  inline void SpiPinInitialiser<TPinPackage,TPeripheralName>::initialise(uint16_t mode,uint16_t direction) {

    GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_SCK,
                                   TPinPackage::Pin_SCK,
                                   Gpio::ALTERNATE_FUNCTION,
                                   (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                   Gpio::PUPD_DOWN,
                                   Gpio::PUSH_PULL,
                                   GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_SCK,TPinPackage::Pin_SCK>::GPIO_AF);

    // in uni-directional mode slave MOSI is free

    if(mode==SPI_Mode_Master || direction==SPI_Direction_2Lines_FullDuplex)
      GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MOSI,
                                     TPinPackage::Pin_MOSI,
                                     Gpio::ALTERNATE_FUNCTION,
                                     (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                     Gpio::PUPD_DOWN,
                                     Gpio::PUSH_PULL,
                                     GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_MOSI,TPinPackage::Pin_MOSI>::GPIO_AF);

    // in uni-directional mode master MISO is free

    if(mode==SPI_Mode_Slave || direction==SPI_Direction_2Lines_FullDuplex)
      GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_MISO,
                                     TPinPackage::Pin_MISO,
                                     Gpio::ALTERNATE_FUNCTION,
                                     (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                     Gpio::PUPD_DOWN,
                                     Gpio::PUSH_PULL,
                                     GpioAlternateFunctionMapper<TPeripheralName,TPinPackage::Port_MISO,TPinPackage::Pin_MISO>::GPIO_AF);

    if(mode==SPI_Mode_Master) {

      // in master mode NSS is an output

      GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_NSS,
                                     TPinPackage::Pin_NSS,
                                     Gpio::OUTPUT,
                                     (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                     Gpio::PUPD_NONE,
                                     Gpio::PUSH_PULL);
    }
    else {

      // in slave mode NSS is an input

      GpioPinInitialiser::initialise((GPIO_TypeDef *)TPinPackage::Port_NSS,
                                     TPinPackage::Pin_NSS,
                                     Gpio::INPUT,
                                     (GPIOSpeed_TypeDef)PeripheralTraits<TPeripheralName>::GPIO_SPEED,
                                     Gpio::PUPD_NONE,
                                     Gpio::PUSH_PULL);
    }
  }
}
