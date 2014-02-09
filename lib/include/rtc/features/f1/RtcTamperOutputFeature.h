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
   * RTC feature to enable output to the tamper pin functionality
   */

  class RtcTamperOutputFeature : public RtcFeatureBase {

    public:
      RtcTamperOutputFeature(RtcBase& rtc);
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcTamperOutputFeature::RtcTamperOutputFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    // Disable the tamper pin because we're using it for an alternate function

    BKP_TamperPinCmd(DISABLE);

    // enable RTC output on tamper

    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
  }
}
