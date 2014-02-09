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
   * Specialisation of the RtcPeripheral for the F1
   */

  class RtcBase {

    public:
      RtcBase();

      void setTick(uint32_t tick) const;
      uint32_t getTick() const;
  };


  /**
   * Constructor
   */

  inline RtcBase::RtcBase() {

    ClockControl<PERIPHERAL_POWER>::On();
    ClockControl<PERIPHERAL_BACKUP>::On();

    PWR_BackupAccessCmd(ENABLE);          // allow backup domain access
    BKP_DeInit();                         // reset backup domain
  }


  /**
   * Set the tick.
   * @param[in] tick The new tick value.
   */

  inline void RtcBase::setTick(uint32_t tick) const {
    RTC_WaitForLastTask();
    RTC_SetCounter(tick);
    RTC_WaitForLastTask();
  }


  /**
   * Get the current tick.
   * @return The current tick.
   */

  inline uint32_t RtcBase::getTick() const {
    RTC_WaitForLastTask();
    return ::RTC_GetCounter();
  }
}
