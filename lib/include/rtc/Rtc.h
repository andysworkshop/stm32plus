/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * RTC template definition
   */

  template<class... Features>
  class Rtc : public RtcBase,
              public Features... {

    public:
      Rtc(uint32_t hourFormat=RTC_HourFormat_24,uint32_t backupValue=0) :
        RtcBase(hourFormat, backupValue),
        Features(static_cast<RtcBase&>(*this))... {
      }
    };
}
