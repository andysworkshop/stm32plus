/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Template feature class to support initialisation of alternate function pins on the F1.
   * Multiple pins can be initialised in one instantiation
   */

  template<GPIOSpeed_TypeDef TSpeed=GPIO_Speed_50MHz,
           Gpio::GpioOutputType TOutputType=Gpio::PUSH_PULL,
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

        // do the initialisation

        initialise(pins,TSpeed,TOutputType);
      }


      /**
       * Alternative to template initialisation - initialise programatically
       * @param speed
       * @param TOutputType
       * @param TPullUpDownType
       * @param pinIds
       */

      void initialise(uint16_t pinIds,
                      GPIOSpeed_TypeDef speed=GPIO_Speed_50MHz,
                      Gpio::GpioOutputType outputType=Gpio::PUSH_PULL) {
        uint8_t i;
        GPIO_InitTypeDef init;

        this->_pinIds=pinIds;

        init.GPIO_Speed=speed;
        init.GPIO_Mode=outputType==OPEN_DRAIN ? GPIO_Mode_AF_OD : GPIO_Mode_AF_PP;
        init.GPIO_Pin=pinIds;

        GPIO_Init(_peripheralAddress,&init);

        // set ourselves as the pin handler in the port base

        for(i=0;i<16;i++)
          if((pinIds & (1<<i))!=0)
            _portBase.setPinHandler(i,this);
      }
  };


  /**
   * Default AFIO helper. This type provides default port speed settings and is only compatible
   * with the F1.
   */

  template<uint8_t... TPins>
  using DefaultAlternateFunctionFeature=AlternateFunctionFeature<GPIO_Speed_50MHz,Gpio::PUSH_PULL,TPins...>;


  /**
   * Types for each of the peripherals supporting AFIO. These are the compatibility types and will work
   * on any MCU. Some of the examples use these to guarantee compatibility.
   */

  template<uint8_t... TPins> using DefaultTim1AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim2AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim3AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim4AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim5AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim8AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim9AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim10AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim11AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim12AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim13AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultTim14AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;

  template<uint8_t... TPins> using DefaultI2C1AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultI2C2AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultI2C3AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;

  template<uint8_t... TPins> using DefaultSpi1AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultSpi2AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultSpi3AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;

  template<uint8_t... TPins> using DefaultUsart1AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultUsart2AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultUsart3AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultUart4AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultUart5AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
  template<uint8_t... TPins> using DefaultUsart6AlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;

  template<uint8_t... TPins> using DefaultFsmcAlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;

  template<uint8_t... TPins> using DefaultSdioAlternateFunctionFeature=DefaultAlternateFunctionFeature<TPins...>;
}
