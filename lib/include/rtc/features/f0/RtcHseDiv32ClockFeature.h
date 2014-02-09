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
   * RTC feature to source the RTC from the HSE clock /32
   */

  template<uint32_t THseFrequency>
  class RtcHseDiv32ClockFeature : public RtcFeatureBase {

    public:
      RtcHseDiv32ClockFeature(RtcBase& rtc);
  };


  /*
   * Typedef for the 8MHz usual case
   */

  typedef RtcHseDiv32ClockFeature<8000000> Rtc8MhzHseDiv32ClockFeature;


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  template<uint32_t THseFrequency>
  inline RtcHseDiv32ClockFeature<THseFrequency>::RtcHseDiv32ClockFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    RTC_InitTypeDef init;

    // on with the HSE

    RCC_HSEConfig(RCC_HSE_ON);

    // wait till HSE is ready

    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY)==RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div32);

    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();

    // Configure the RTC hour format and prescaler for the supplied clock

    init.RTC_AsynchPrediv=0x7F;
    init.RTC_SynchPrediv=((THseFrequency/32)/128)-1;  // HSE freq (Hz / 32 / 128) - 1
    init.RTC_HourFormat=_rtc.getHourFormat();

    RTC_Init(&init);
  }
}
