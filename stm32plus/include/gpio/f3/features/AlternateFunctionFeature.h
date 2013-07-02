/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F3
#error This class can only be used with the STM32F3 series
#endif


namespace stm32plus {


  /**
   * Template feature class to support initialisation of alternate function pins on the F3.
   * Multiple pins can be initialised in one instantiation
   */

  template<uint8_t TAfSelection,
           GPIOSpeed_TypeDef TSpeed=GPIO_Speed_50MHz,
           Gpio::GpioOutputType TOutputType=Gpio::PUSH_PULL,
           Gpio::GpioPullUpDownType TPullUpDownType=Gpio::PUPD_NONE,
           uint8_t p1=16,uint8_t p2=16,uint8_t p3=16,uint8_t p4=16,
           uint8_t p5=16,uint8_t p6=16,uint8_t p7=16,uint8_t p8=16,
           uint8_t p9=16,uint8_t p10=16,uint8_t p11=16,uint8_t p12=16,
           uint8_t p13=16,uint8_t p14=16,uint8_t p15=16,uint8_t p16=16>
  class AlternateFunctionFeature : public Gpio {

    public:

    /**
     * Constructor
     * @param port the port reference
     */

      AlternateFunctionFeature(GpioPortBase& port)
        : Gpio(port) {

        uint32_t pins;

        // p1..16 are constants. the compiler will eliminate the impossible branches
        // so the code generated here is minimal. In an optimised build all the 16 conditionals below
        // are eliminated and the compiler generates a single load instruction with a constant.

        pins=0;

        if(p1!=16)
          pins|=1 << p1;

        if(p2!=16)
          pins|=1 << p2;

        if(p3!=16)
          pins|=1 << p3;

        if(p4!=16)
          pins|=1 << p4;

        if(p5!=16)
          pins|=1 << p5;

        if(p6!=16)
          pins|=1 << p6;

        if(p7!=16)
          pins|=1 << p7;

        if(p8!=16)
          pins|=1 << p8;

        if(p9!=16)
          pins|=1 << p9;

        if(p10!=16)
          pins|=1 << p10;

        if(p11!=16)
          pins|=1 << p11;

        if(p12!=16)
          pins|=1 << p12;

        if(p13!=16)
          pins|=1 << p13;

        if(p14!=16)
          pins|=1 << p14;

        if(p15!=16)
          pins|=1 << p15;

        if(p16!=16)
          pins|=1 << p16;

        // do the initialisation in the non-template base

        initialiseAF(pins,TAfSelection,TSpeed,TOutputType,TPullUpDownType);
      }
  };


  /**
   * Default alternate function helper
   */

  template<uint8_t TAfSelection,
           uint8_t p1=16,uint8_t p2=16,uint8_t p3=16,uint8_t p4=16,
           uint8_t p5=16,uint8_t p6=16,uint8_t p7=16,uint8_t p8=16,
           uint8_t p9=16,uint8_t p10=16,uint8_t p11=16,uint8_t p12=16,
           uint8_t p13=16,uint8_t p14=16,uint8_t p15=16,uint8_t p16=16>
  struct DefaultAlternateFunctionFeature : AlternateFunctionFeature<TAfSelection,GPIO_Speed_50MHz,Gpio::PUSH_PULL,Gpio::PUPD_NONE,
                                                                    p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16> {
    DefaultAlternateFunctionFeature(GpioPortBase& port) : AlternateFunctionFeature<TAfSelection,GPIO_Speed_50MHz,Gpio::PUSH_PULL,Gpio::PUPD_NONE,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16>(port) {}
  };
}
