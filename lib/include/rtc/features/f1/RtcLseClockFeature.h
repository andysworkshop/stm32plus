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
   * RTC feature to source the RTC from the LSE clock
   */

  class RtcLseClockFeature : public RtcFeatureBase {

    public:
      RtcLseClockFeature(RtcBase& rtc);
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcLseClockFeature::RtcLseClockFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    // on with the LSE

    RCC_LSEConfig(RCC_LSE_ON);

    // wait till LSE is ready

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);   // select the RTC Clock Source
    RCC_RTCCLKCmd(ENABLE);                    // enable RTC Clock

    RTC_WaitForSynchro();                     // wait for RTC registers synchronization
    RTC_WaitForLastTask();                    // wait until last write operation on RTC registers has finished

    RTC_SetPrescaler(32767);                  // set RTC prescaler: set RTC period to 1s
    RTC_WaitForLastTask();                    // wait until last write operation on RTC registers has finished
  }
}
