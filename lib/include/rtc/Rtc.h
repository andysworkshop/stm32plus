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
      Rtc(uint32_t hourFormat=0,uint32_t backupValue=0) :
        RtcBase(hourFormat, backupValue),
        Features(static_cast<RtcBase&>(*this))... {

        // remember successful RTC initialization after features have run
        // only useful with LSE which is part of the backup domain

        if(backupValue) {
#if defined(STM32PLUS_F1)
          BKP_WriteBackupRegister(BKP_DR1,backupValue);
#else
          RTC_WriteBackupRegister(RTC_BKP_DR0,backupValue);
#endif
        }
      }
    };
}
