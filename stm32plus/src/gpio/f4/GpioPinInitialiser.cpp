/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/gpio.h"

#if defined(STM32PLUS_F4)

namespace stm32plus {

  namespace GpioPinInitialiser {

    /**
     * Simple initialiser for input pins that includes AFIO selection and no output type
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pins,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed,
                    Gpio::GpioPullUpDownType pupdType,
                    uint8_t afSelection) {

      initialise(port,pins,mode,speed,pupdType,Gpio::PUSH_PULL,afSelection);
    }


    /**
     * Simple initialiser that includes AFIO selection
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pins,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed,
                    Gpio::GpioPullUpDownType pupdType,
                    Gpio::GpioOutputType outputType,
                    uint8_t afSelection) {

      uint32_t bit;
      uint8_t source;

      initialise(port,pins,mode,speed,pupdType,outputType);

      // need to configure each of the selected pins for AF

      for(bit=1,source=0;bit<=0x8000;bit<<=1,source++)
        if((pins & bit)!=0)
          GPIO_PinAFConfig(port,source,afSelection);
    }


    /**
     * Simple pin initialiser when you don't need or want the template support offered by GpioPort
     * and its features.
     * @param port
     * @param pins
     * @param mode
     * @param speed
     * @param pupdType
     * @param outputType
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pins,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed,
                    Gpio::GpioPullUpDownType pupdType,
                    Gpio::GpioOutputType outputType) {

      GPIO_InitTypeDef init;

      // clock on

      RCC_AHB1PeriphClockCmd(port==GPIOA ? RCC_AHB1Periph_GPIOA :
                             port==GPIOB ? RCC_AHB1Periph_GPIOB :
                             port==GPIOC ? RCC_AHB1Periph_GPIOC :
                             port==GPIOD ? RCC_AHB1Periph_GPIOD :
                             port==GPIOE ? RCC_AHB1Periph_GPIOE :
                             port==GPIOF ? RCC_AHB1Periph_GPIOF :
                             port==GPIOG ? RCC_AHB1Periph_GPIOG :
                             port==GPIOH ? RCC_AHB1Periph_GPIOH :
                             RCC_AHB1Periph_GPIOI
                             ,ENABLE);

      // initialise

      init.GPIO_Pin=pins;
      init.GPIO_Speed=speed;

      switch(mode) {

        case Gpio::OUTPUT:
          init.GPIO_Mode=GPIO_Mode_OUT;
          break;

        case Gpio::INPUT:
          init.GPIO_Mode=GPIO_Mode_IN;
          break;

        case Gpio::ANALOG:
          init.GPIO_Mode=GPIO_Mode_AN;
          break;

        case Gpio::ALTERNATE_FUNCTION:
          init.GPIO_Mode=GPIO_Mode_AF;
          break;
      }

      switch(pupdType) {

        case Gpio::PUPD_NONE:
          init.GPIO_PuPd=GPIO_PuPd_NOPULL;
          break;

        case Gpio::PUPD_UP:
          init.GPIO_PuPd=GPIO_PuPd_UP;
          break;

        case Gpio::PUPD_DOWN:
          init.GPIO_PuPd=GPIO_PuPd_DOWN;
          break;
      }

      switch(outputType) {

        case Gpio::OPEN_DRAIN:
          init.GPIO_OType=GPIO_OType_OD;
          break;

        case Gpio::PUSH_PULL:
          init.GPIO_OType=GPIO_OType_PP;
          break;
      }

      GPIO_Init(port,&init);
    }
  }
}

#endif
