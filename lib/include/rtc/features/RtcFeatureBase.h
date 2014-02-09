/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for RTC features
   */

  class RtcFeatureBase {
    protected:
      RtcBase& _rtc;

    public:
      RtcFeatureBase(RtcBase& rtc);
      operator RtcBase&();
  };


  /**
   * Constructor
   * @param rtc
   */

  inline RtcFeatureBase::RtcFeatureBase(RtcBase& rtc)
    : _rtc(rtc) {
  }


  /**
   * Cast to RtcBase reference
   */

  inline RtcFeatureBase::operator RtcBase&() {
    return _rtc;
  }
}
