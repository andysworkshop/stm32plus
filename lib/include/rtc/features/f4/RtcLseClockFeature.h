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
   * RTC feature to source the RTC from the LSE clock (32.768 kHz)
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

    RTC_InitTypeDef init;

    if(rtc.survived())
      return;             // already configured from earlier boot

    // on with the LSE

    RCC_LSEConfig(RCC_LSE_ON);

    // wait till LSE is ready. can hang if LSE is not working

    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();

    // Configure the RTC hour format and prescaler for a 32.768kHz clock

    init.RTC_AsynchPrediv=0x7F;
    init.RTC_SynchPrediv=0xFF;        // LSI freq (Hz / 128)-1 = (32768 / 128 ) -1 = 255
    init.RTC_HourFormat=_rtc.getHourFormat();

    RTC_Init(&init);
  }
}
