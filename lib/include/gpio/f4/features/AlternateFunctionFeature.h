/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Template feature class to support initialisation of alternate function pins on the F4.
   * Multiple pins can be initialised in one instantiation
   */

  template<uint8_t TAfSelection,
           GPIOSpeed_TypeDef TSpeed=GPIO_Speed_50MHz,
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
  using DefaultAlternateFunctionFeature=AlternateFunctionFeature<TAfSelection,GPIO_Speed_50MHz,Gpio::PUSH_PULL,Gpio::PUPD_NONE,TPins...>;


  /**
   * Types for each of the peripherals supporting AFIO. These are the compatibility types and will work
   * on any MCU. Some of the examples use these to guarantee compatibility.
   */

  template<uint8_t... TPins> using DefaultTim1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM1,TPins...>;
  template<uint8_t... TPins> using DefaultTim2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM2,TPins...>;
  template<uint8_t... TPins> using DefaultTim3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM3,TPins...>;
  template<uint8_t... TPins> using DefaultTim4AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM4,TPins...>;
  template<uint8_t... TPins> using DefaultTim5AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM5,TPins...>;
  template<uint8_t... TPins> using DefaultTim8AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM8,TPins...>;
  template<uint8_t... TPins> using DefaultTim9AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM9,TPins...>;
  template<uint8_t... TPins> using DefaultTim10AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM10,TPins...>;
  template<uint8_t... TPins> using DefaultTim11AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM11,TPins...>;
  template<uint8_t... TPins> using DefaultTim12AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM12,TPins...>;
  template<uint8_t... TPins> using DefaultTim13AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM13,TPins...>;
  template<uint8_t... TPins> using DefaultTim14AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_TIM14,TPins...>;

  template<uint8_t... TPins> using DefaultI2C1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_I2C1,TPins...>;
  template<uint8_t... TPins> using DefaultI2C2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_I2C2,TPins...>;
  template<uint8_t... TPins> using DefaultI2C3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_I2C3,TPins...>;

  template<uint8_t... TPins> using DefaultSpi1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SPI1,TPins...>;
  template<uint8_t... TPins> using DefaultSpi2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SPI2,TPins...>;
  template<uint8_t... TPins> using DefaultSpi3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SPI3,TPins...>;

  template<uint8_t... TPins> using DefaultUsart1AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART1,TPins...>;
  template<uint8_t... TPins> using DefaultUsart2AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART2,TPins...>;
  template<uint8_t... TPins> using DefaultUsart3AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART3,TPins...>;
  template<uint8_t... TPins> using DefaultUart4AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_UART4,TPins...>;
  template<uint8_t... TPins> using DefaultUart5AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_UART5,TPins...>;
  template<uint8_t... TPins> using DefaultUsart6AlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_USART6,TPins...>;

  // FSMC/FMC depending on device

#if defined(STM32F40_41xxx)
  template<uint8_t... TPins> using DefaultFsmcAlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_FSMC,TPins...>;
#endif

#if defined(STM32F427_437xx) || defined(STM32F429_439xx)
  template<uint8_t... TPins> using DefaultFmcAlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_FMC,TPins...>;
#endif

  template<uint8_t... TPins> using DefaultSdioAlternateFunctionFeature=DefaultAlternateFunctionFeature<GPIO_AF_SDIO,TPins...>;
}
