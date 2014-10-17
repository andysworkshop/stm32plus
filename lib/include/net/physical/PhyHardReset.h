/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /*
     * Provides optional support for issuing a hard reset to the PHY via
     * a GPIO pin. The PHY itself must support phyHardReset() and phyInitialiseReset()
     * methods. The TResetGpio template parameter should be one of the GpioPinMap.h types
     * e.g. gpio::PB14
     */

    template<class TPhy,class TResetGpio>
    class PhyHardReset {

      public:
        struct Parameters {   ///<! required by the initialise() contract but not used
        };

      public:
        bool initialise(Parameters& params,NetworkUtilityObjects& netutils,TPhy& phy);
        bool startup();
    };


    /**
     * Initialise the class
     * @return true
     */

    template<class TPhy,class TResetGpio>
    inline bool PhyHardReset<TPhy,TResetGpio>::initialise(Parameters& /* params */,NetworkUtilityObjects& /* netutils */,TPhy& phy) {

      // initialise the reset pin for output

      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TResetGpio::Port),
          TResetGpio::Pin,
          Gpio::OUTPUT);

      // set up a pinref

      GpioPinRef resetPin(
          reinterpret_cast<GPIO_TypeDef *>(TResetGpio::Port),
          TResetGpio::Pin
        );

      // ask the PHY to do the reset. the PHY needs to do it because we
      // don't know the correct up/down/wait sequence

      phy.phyHardReset(resetPin);
      return true;
    }


    /**
     * Startup, nothing to do
     * @return true
     */

    template<class TPhy,class TResetGpio>
    inline bool PhyHardReset<TPhy,TResetGpio>::startup() {
      return true;
    }
  }
}
