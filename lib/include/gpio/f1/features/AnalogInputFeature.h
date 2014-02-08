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
   * Template feature class to support initialisation of analog input pins on the F1.
   * Multiple pins can be initialised in one instantiation
   */

  template<uint8_t... TPins>
  class AnalogInputFeature : public Gpio {

    public:

    /**
     * Constructor
     * @param port the port reference
     */

      AnalogInputFeature(GpioPortBase& port)
        : Gpio(port) {

        uint32_t pins;

        // recurse to get the pin mask

        pins=0;
        GpioPinMerge<TPins...>(pins);

        // do the initialisation

        initialise(pins);
      }


      /**
       * Alternative to template initialisation - initialise programatically
       * @param pinIds
       */

      void initialise(uint16_t pinIds) {

        uint8_t i;
        GPIO_InitTypeDef init;

        this->_pinIds=pinIds;

        init.GPIO_Pin=pinIds;
        init.GPIO_Mode=GPIO_Mode_AIN;

        GPIO_Init(_peripheralAddress,&init);

        // set ourselves as the pin handler in the port base

        for(i=0;i<16;i++)
          if((pinIds & (1<<i))!=0)
            _portBase.setPinHandler(i,this);
      }
  };
}
