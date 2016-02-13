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
    enum {
       BKP_ALWAYS_RESET = 0,           // Old stm32plus behavior
       NOT_SURVIVED_FLAG = 0x1000000,  // Stored in _hourFormat
       HOUR_FORMAT_MASK = 0x0000ffff   // Hour format values are 0 and 64
     };

    protected:
      bool _survived;

    public:
      RtcBase(uint32_t ignored=0,uint32_t backupValue=BKP_ALWAYS_RESET);

      void setTick(uint32_t tick) const;
      uint32_t getTick() const;
      bool survived() const;  // true if RTC configuration survived reboot
  };


  /**
   * Constructor
   */

  inline RtcBase::RtcBase(uint32_t ignored, uint32_t backupValue)
    : _survived(true) {

    ignored++;

    ClockControl<PERIPHERAL_POWER>::On();
    ClockControl<PERIPHERAL_BACKUP>::On();

    PWR_BackupAccessCmd(ENABLE);          // allow backup domain access

    // derived class should set the backup value after configuring the LSE

    if(backupValue==BKP_ALWAYS_RESET || BKP_ReadBackupRegister(BKP_DR1)!=backupValue) {
      BKP_DeInit();     // reset backup domain
      _survived=false;
    }
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


  /**
   * Return whether the RTC configuration survived reset, indicating
   * whether the backup domain was reset or not.
   * @return true if the RTC retained its configuration from a prior boot
   */

  inline bool RtcBase::survived() const {
    return _survived;
  }
}
