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
   * RTC feature to source the RTC from the LSI clock. ST attempt to measure the LSI clock
   * in their examples so we optionally provide that functionality as well.
   */

  template<class TLsiFrequencyProvider>
  class RtcLsiClockFeature : public RtcFeatureBase {

    public:
      RtcLsiClockFeature(RtcBase& rtc);
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  template<class TLsiFrequencyProvider>
  inline RtcLsiClockFeature<TLsiFrequencyProvider>::RtcLsiClockFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    // on with the LSI

    RCC_LSICmd(ENABLE);

    // wait till LSE is ready

    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();
  }
}
