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
   * Template feature class to support initialisation of alternate function pins on the F0.
   * Multiple pins can be initialised in one instantiation
   */

  template<uint8_t TAfSelection,
           GPIOSpeed_TypeDef TSpeed=GPIO_Speed_Level_3,
           Gpio::GpioOutputType TOutputType=Gpio::PUSH_PULL,
           Gpio::GpioPullUpDownType TPullUpDownType=Gpio::PUPD_NONE,
           uint8_t... TPins>
  class AlternateFunctionFeature : public Gpio {

    public:

    /**
     * Constructor
     * @param port the port reference
     */

      AlternateFunctionFeature(GpioPortBase& port)
        : Gpio(port) {

        uint32_t pins;

        // recurse to get the pin mask

        pins=0;
        GpioPinMerge<TPins...>(pins);

        // do the initialisation in the non-template base

        initialiseAF(pins,TAfSelection,TSpeed,TOutputType,TPullUpDownType);
      }
  };


  /**
   * Default alternate function helper. This provides default port speed settings and is only
   * compatible with the F4.
   */

  template<uint8_t TAfSelection,uint8_t... TPins>
  using DefaultAlternateFunctionFeature=AlternateFunctionFeature<TAfSelection,GPIO_Speed_Level_3,Gpio::PUSH_PULL,Gpio::PUPD_NONE,TPins...>;


  /**
   * Types for each of the peripherals supporting AFIO. These are the compatibility types and will work
   * on any MCU. Some of the examples use these to guarantee compatibility.
   */

  template<uint8_t... TPins> using DefaultTim1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_2,TPins...>;
  template<uint8_t... TPins> using DefaultTim2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_2,TPins...>;
  template<uint8_t... TPins> using DefaultTim3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_1,TPins...>;
  template<uint8_t... TPins> using DefaultTim14AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_0,TPins...>;
  template<uint8_t... TPins> using DefaultTim15AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_3,TPins...>;
  template<uint8_t... TPins> using DefaultTim16AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_0,TPins...>;
  template<uint8_t... TPins> using DefaultTim17AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_0,TPins...>;

  template<uint8_t... TPins> using DefaultI2C1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_3,TPins...>;
  template<uint8_t... TPins> using DefaultI2C2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_1,TPins...>;

  template<uint8_t... TPins> using DefaultSpi1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_0,TPins...>;
  template<uint8_t... TPins> using DefaultSpi2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_0,TPins...>;

  template<uint8_t... TPins> using DefaultUsart1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_1,TPins...>;
  template<uint8_t... TPins> using DefaultUsart2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_1,TPins...>;
}
