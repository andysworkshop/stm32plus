/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Support for configuring the analog watchdog feature. The AWD allows you to
   * trigger an interrupt if a voltage crosses pre-defined upper or lower bounds.
   * This feature allows you to configure all enabled channels for monitoring.
   *
   * This feature would normally be combined with AdcInterruptFeature in your
   * ADC declaration.
   */

  template<uint16_t TLow,uint16_t THigh>
  struct AdcAllChannelsAnalogWatchdogFeature : AdcAnalogWatchdogFeature<TLow,THigh> {

    public:

      /**
       * Constructor
       * @param adc Reference to the main ADC class
       */

      AdcAllChannelsAnalogWatchdogFeature(Adc& adc)
       : AdcAnalogWatchdogFeature<TLow,THigh>(adc) {
      }


      /**
       * Post construction initialisation
       */

      void initialise() {

        // call the base class

        AdcAnalogWatchdogFeature<TLow,THigh>::initialise();
        ADC_AnalogWatchdogSingleChannelCmd(this->_adc,DISABLE);     // clears AWDSGL
      }
  };
}
