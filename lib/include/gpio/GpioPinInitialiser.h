/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  namespace GpioPinInitialiser {

    /**
     * Initialisation for non-AF pins
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pins,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed=GPIO_Speed_50MHz,
                    Gpio::GpioPullUpDownType pupdType=Gpio::PUPD_NONE,
                    Gpio::GpioOutputType outputType=Gpio::PUSH_PULL);

    /**
     * Initialisation for analog pins
     */

    void initialise(GPIO_TypeDef *port,uint16_t pins);


    /**
     * Initialisation for AF input pins
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pin,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed,
                    Gpio::GpioPullUpDownType pupdType,
                    uint8_t afSelection);

    /**
     * Initialisation for AF input/output pins
     */

    void initialise(GPIO_TypeDef *port,
                    uint16_t pin,
                    Gpio::GpioModeType mode,
                    GPIOSpeed_TypeDef speed,
                    Gpio::GpioPullUpDownType pupdType,
                    Gpio::GpioOutputType outputType,
                    uint8_t afSelection);

    /**
     * Initialiser for analog input pins
     */

    void initialise(GPIO_TypeDef *port,uint16_t pins);
  }
}
