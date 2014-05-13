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
   * Support for configuring the analog watchdog feature. The AWD allows you to
   * trigger an interrupt if a voltage crosses pre-defined upper or lower bounds.
   * This feature allows you to configure all enabled channels for monitoring.
   *
   * This feature would normally be combined with AdcInterruptFeature in your
   * ADC declaration.
   */

  template<AdcChannelType TChannelType,uint16_t TLow,uint16_t THigh>
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
       * Enable the watchdog on all channels of the templated type
       */

      void enableAnalogWatchdog() {

        static_assert(TChannelType==AdcChannelType::Regular ||
                      TChannelType==AdcChannelType::Injected ||
                      TChannelType==AdcChannelType::RegularAndInjected,"Invalid channel type");

        ADC_AnalogWatchdogCmd(
            this->_adc,
            TChannelType==AdcChannelType::Regular ? ADC_AnalogWatchdog_AllRegEnable :
            TChannelType==AdcChannelType::Injected ? ADC_AnalogWatchdog_AllInjecEnable :
            ADC_AnalogWatchdog_AllRegAllInjecEnable);
      }

  };
}
