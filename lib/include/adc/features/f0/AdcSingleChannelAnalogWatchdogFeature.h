/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * Support for configuring the analog watchdog feature. The AWD allows you to
   * trigger an interrupt if a voltage crosses pre-defined upper or lower bounds.
   * This feature allows you to configure a single channel for monitoring.
   *
   * This feature would normally be combined with AdcInterruptFeature in your
   * ADC declaration.
   */

  template<uint16_t TChannelNumber,uint16_t TLow,uint16_t THigh>
  struct AdcSingleChannelAnalogWatchdogFeature : AdcAnalogWatchdogFeature<TLow,THigh> {

    public:

      /**
       * Constructor
       * @param adc Reference to the main ADC class
       */

      AdcSingleChannelAnalogWatchdogFeature(Adc& adc)
       : AdcAnalogWatchdogFeature<TLow,THigh>(adc) {
      }


      /**
       * Post construction initialisation
       */

      void initialise() {

        // call the base class

        AdcAnalogWatchdogFeature<TLow,THigh>::initialise();

        // set the channel and enable single mode. actual guarding will not start
        // until enableAnalogWatchdog() is called.

        ADC_AnalogWatchdogSingleChannelConfig(this->_adc,getChannelCode());
        ADC_AnalogWatchdogSingleChannelCmd(this->_adc,ENABLE);
      }


      /**
       * Get the channel code from the channel number. Operates on constants so should
       * reduce to a simple "return X" on compilation.
       */

      constexpr uint32_t getChannelCode() const {

        // unfortunately the codes are not a simple bit sequence

        return TChannelNumber==0 ? ADC_AnalogWatchdog_Channel_0 :
               TChannelNumber==1 ? ADC_AnalogWatchdog_Channel_1 :
               TChannelNumber==2 ? ADC_AnalogWatchdog_Channel_2 :
               TChannelNumber==3 ? ADC_AnalogWatchdog_Channel_3 :
               TChannelNumber==4 ? ADC_AnalogWatchdog_Channel_4 :
               TChannelNumber==5 ? ADC_AnalogWatchdog_Channel_5 :
               TChannelNumber==6 ? ADC_AnalogWatchdog_Channel_6 :
               TChannelNumber==7 ? ADC_AnalogWatchdog_Channel_7 :
               TChannelNumber==8 ? ADC_AnalogWatchdog_Channel_8 :
               TChannelNumber==9 ? ADC_AnalogWatchdog_Channel_9 :
               TChannelNumber==10 ? ADC_AnalogWatchdog_Channel_10 :
               TChannelNumber==11 ? ADC_AnalogWatchdog_Channel_11 :
               TChannelNumber==12 ? ADC_AnalogWatchdog_Channel_12 :
               TChannelNumber==13 ? ADC_AnalogWatchdog_Channel_13 :
               TChannelNumber==14 ? ADC_AnalogWatchdog_Channel_14 :
               TChannelNumber==15 ? ADC_AnalogWatchdog_Channel_15 :
               TChannelNumber==16 ? ADC_AnalogWatchdog_Channel_16 :
               TChannelNumber==17 ? ADC_AnalogWatchdog_Channel_17 :
               ADC_AnalogWatchdog_Channel_18;
      }
  };
}
