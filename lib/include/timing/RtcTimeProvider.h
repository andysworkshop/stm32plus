/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Time provider that uses the real time clock
   */

  class RtcTimeProvider : public TimeProvider {
    protected:
      const RtcBase& _rtc;

    public:

      /**
       * Constructor.
       * @param[in] rtc_ The RTC. Caller supplied parameter, must not go out of scope.
       */

      RtcTimeProvider(const RtcBase& rtc) :
        _rtc(rtc) {
      }

      /**
       * Virtual destructor
       */

      virtual ~RtcTimeProvider() {}

      /**
       * Return the time. Actually returns the tick-count from the RTC.
       * @return The tick count.
       */

      virtual time_t getTime() const override {
        return _rtc.getTick();
      }
  };

}
