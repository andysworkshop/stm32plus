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
   * Template feature class to support initialisation of output pins on the F1.
   * Multiple pins can be initialised in one instantiation.
   *
   * 16 as the "no-pin" constant and uint32_t as the pin id data type is a workaround for
   * a bogus gcc shift-size warning (gcc bug 44811)
   */

  template<GPIOSpeed_TypeDef TSpeed=GPIO_Speed_50MHz,
           Gpio::GpioOutputType TOutputType=Gpio::PUSH_PULL,
           Gpio::GpioPullUpDownType TPullUpDownType=Gpio::PUPD_NONE,              // not applicable to the F1
           uint8_t... TPins>
  class DigitalOutputFeature : public Gpio {

    public:

    /**
     * Constructor
     * @param port the port reference
     */

      DigitalOutputFeature(GpioPortBase& port)
        : Gpio(port) {

        uint32_t pins;

        // recurse to get the pin mask

        pins=0;
        GpioPinMerge<TPins...>(pins);

        // do the initialisation

        initialise(pins,TSpeed,TOutputType,TPullUpDownType);
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
                      Gpio::GpioOutputType outputType=Gpio::PUSH_PULL,
                      Gpio::GpioPullUpDownType pullUpDownType=Gpio::PUPD_NONE) {
        uint8_t i;
        GPIO_InitTypeDef init;

        (void)pullUpDownType;

        this->_pinIds=pinIds;

        init.GPIO_Speed=speed;
        init.GPIO_Mode=outputType==OPEN_DRAIN ? GPIO_Mode_Out_OD : GPIO_Mode_Out_PP;
        init.GPIO_Pin=pinIds;

        GPIO_Init(_peripheralAddress,&init);

        // set ourselves as the pin handler in the port base

        for(i=0;i<16;i++)
          if((pinIds & (1<<i))!=0)
            _portBase.setPinHandler(i,this);
      }
  };


  /**
   * Default digital output helper
   */

  template<uint8_t... TPins >
  using DefaultDigitalOutputFeature=DigitalOutputFeature<GPIO_Speed_50MHz,Gpio::PUSH_PULL,Gpio::PUPD_NONE,TPins...>;
}
