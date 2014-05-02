/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/gpio.h"

#if defined(STM32PLUS_F1)

namespace stm32plus {

  namespace GpioPinInitialiser {

    /**
     * Simple initialiser for input pins that includes AFIO selection and no output type
     * The AFIO selection is ignored on the F1
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
     * Simple initialiser that includes AFIO selection that is ignored on the F1
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pins,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed,
                    Gpio::GpioPullUpDownType pupdType,
                    Gpio::GpioOutputType outputType,
                    uint8_t /* afSelection */) {

      initialise(port,pins,mode,speed,pupdType,outputType);
    }


    /**
     * Simple initialiser for analog input pins
     */

    void initialise(GPIO_TypeDef *port,uint16_t pins) {

      initialise(port,pins,Gpio::ANALOG,GPIO_Speed_50MHz,Gpio::PUPD_NONE,Gpio::PUSH_PULL);
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

      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

      RCC_APB2PeriphClockCmd(port==GPIOA ? RCC_APB2Periph_GPIOA :
                             port==GPIOB ? RCC_APB2Periph_GPIOB :
                             port==GPIOC ? RCC_APB2Periph_GPIOC :
                             port==GPIOD ? RCC_APB2Periph_GPIOD :
                             port==GPIOE ? RCC_APB2Periph_GPIOE :
                             port==GPIOF ? RCC_APB2Periph_GPIOF :
                             RCC_APB2Periph_GPIOG
                             ,ENABLE);

      // initialise

      init.GPIO_Pin=pins;
      init.GPIO_Speed=speed;

      switch(mode) {

        case Gpio::OUTPUT:
          init.GPIO_Mode=outputType==Gpio::OPEN_DRAIN ? GPIO_Mode_Out_OD : GPIO_Mode_Out_PP;
          break;

        case Gpio::INPUT:
          init.GPIO_Mode=pupdType==Gpio::PUPD_NONE ? GPIO_Mode_IN_FLOATING : pupdType==Gpio::PUPD_UP ? GPIO_Mode_IPD : GPIO_Mode_IPU;
          break;

        case Gpio::ANALOG:
          init.GPIO_Mode=GPIO_Mode_AIN;
          break;

        case Gpio::ALTERNATE_FUNCTION:
          init.GPIO_Mode=outputType==Gpio::PUSH_PULL ? GPIO_Mode_AF_PP : GPIO_Mode_AF_OD;
          break;
      }

      GPIO_Init(port,&init);
    }
  }
}

#endif
