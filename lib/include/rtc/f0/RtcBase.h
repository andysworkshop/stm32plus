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
   * Specialisation of the RtcPeripheral for the F4
   */

  class RtcBase {
    enum {
      BKP_ALWAYS_RESET = 0,           // Old stm32plus behavior
      NOT_SURVIVED_FLAG = 0x1000000,  // Stored in _hourFormat
      HOUR_FORMAT_MASK = 0x0000ffff   // Hour format values are 0 and 64
    };

    protected:
      uint32_t _hourFormat;

    public:
      RtcBase(uint32_t hourFormat=RTC_HourFormat_24,uint32_t backupValue=BKP_ALWAYS_RESET);

      void setTime(uint8_t hours,uint8_t minutes,uint8_t seconds,uint8_t am_pm=RTC_H12_AM) const;
      void getTime(uint8_t& hours,uint8_t& minutes,uint8_t& seconds,uint8_t& am_pm) const;

      void setDate(uint8_t year,uint8_t month,uint8_t dayOfMonth,uint8_t dayOfWeek) const;
      void getDate(uint8_t& year,uint8_t& month,uint8_t& dayOfMonth,uint8_t& dayOfWeek) const;

      uint32_t getTick() const;
      void setTick(uint32_t tick) const;

      uint32_t getHourFormat() const;
      bool survived() const;        // true if RTC configuration survived reboot
  };


  /**
   * Constructor.
   * If backupValue == 0, the RTC and backup domain will be reset.
   * If backupValue is non-zero, the RTC and backup domain will only be reset
   * if RTC_BKP_DR0 != backupValue.
   */

  inline RtcBase::RtcBase(uint32_t hourFormat, uint32_t backupValue)
    : _hourFormat(hourFormat) {

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    PWR_BackupAccessCmd(ENABLE);          // allow backup domain access

    // Ideally we would only set the backup value after configuring the LSE,
    // and the number would depend on the chosen oscillator & settings.
    // Otherwise re-flashing the firmware may not behave as expected.
    // Also reset clock if the date and time are all zeros.
    uint8_t year, month, dayOfMonth, dayOfWeek, hours, minutes, seconds, am_pm;
    this->getDate(year, month, dayOfMonth, dayOfWeek);
    this->getTime(hours, minutes, seconds, am_pm);
    if(backupValue==BKP_ALWAYS_RESET || RTC_ReadBackupRegister(RTC_BKP_DR0)!=backupValue ||
      (year|month|dayOfMonth|dayOfWeek|hours|minutes|seconds|am_pm)==0) {
      // reset the backup domain

      RCC_BackupResetCmd(ENABLE);
      RCC_BackupResetCmd(DISABLE);

      _hourFormat|=NOT_SURVIVED_FLAG;
    }
  }


  /**
   * Set the time
   * @param hours
   * @param minutes
   * @param seconds
   * @param am_pm not relevant if in 24 hour format
   */

  inline void RtcBase::setTime(uint8_t hours,uint8_t minutes,uint8_t seconds,uint8_t am_pm) const {

    RTC_TimeTypeDef tset;

    tset.RTC_H12=am_pm;
    tset.RTC_Hours=hours;
    tset.RTC_Minutes=minutes;
    tset.RTC_Seconds=seconds;

    RTC_SetTime(RTC_Format_BIN,&tset);
  }


  /**
   * Get the current time
   * @param hours
   * @param minutes
   * @param seconds
   * @param am_pm
   * @return
   */

  inline void RtcBase::getTime(uint8_t& hours,uint8_t& minutes,uint8_t& seconds,uint8_t& am_pm) const {

    RTC_TimeTypeDef tget;

    RTC_GetTime(RTC_Format_BIN,&tget);

    am_pm=tget.RTC_H12;
    hours=tget.RTC_Hours;
    minutes=tget.RTC_Minutes;
    seconds=tget.RTC_Seconds;
  }


  /**
   * Set the date
   * @param year (0..99)
   * @param month (1..12)
   * @param dayOfMonth (1..31)
   * @param dayOfWeek (1..7) where 1=Monday
   */

  inline void RtcBase::setDate(uint8_t year,uint8_t month,uint8_t dayOfMonth,uint8_t dayOfWeek) const {

    RTC_DateTypeDef sdate;

    sdate.RTC_Year=year;
    sdate.RTC_Month=month;
    sdate.RTC_Date=dayOfMonth;
    sdate.RTC_WeekDay=dayOfWeek;

    RTC_SetDate(RTC_Format_BIN,&sdate);
  }


  /**
   * Get the date
   * @param year
   * @param month (1..12)
   * @param dayOfMonth (1..31)
   * @param dayOfWeek (1..7) where 1=Monday
   */

  inline void RtcBase::getDate(uint8_t& year,uint8_t& month,uint8_t& dayOfMonth,uint8_t& dayOfWeek) const {

    RTC_DateTypeDef sdate;

    RTC_GetDate(RTC_Format_BIN,&sdate);

    year=sdate.RTC_Year;
    month=sdate.RTC_Month;
    dayOfMonth=sdate.RTC_Date;
    dayOfWeek=sdate.RTC_WeekDay;
  }


  /**
   * Get the hour format
   * @return RTC_HourFormat_24 / RTC_HourFormat_12
   */

  inline uint32_t RtcBase::getHourFormat() const {
    return _hourFormat & HOUR_FORMAT_MASK;
  }


  /**
   * Return whether the RTC configuration survived reset, indicating
   * whether the backup domain was reset or not.
   * @return true if the RTC retained its configuration from a prior boot
   */

  inline bool RtcBase::survived() const {
    return !(_hourFormat & NOT_SURVIVED_FLAG);
  }


  /**
   * Get the tick value from the RTC. ticks == time_t value. Assumes that 00 year = 2000.
   * Is IRQ-safe.
   * @return The number of ticks (time_t)
   */

  inline uint32_t RtcBase::getTick() const {

    RTC_DateTypeDef rdate;
    RTC_TimeTypeDef rtime;
    struct tm tstruct;

    // get time and date

    RTC_GetTime(RTC_Format_BIN,&rtime);
    RTC_GetDate(RTC_Format_BIN,&rdate);

    // if in 24 hour clock or hour is less than 12 then believe the hours

    if(_hourFormat==RTC_HourFormat_24 || rtime.RTC_H12==RTC_H12_AM)
      tstruct.tm_hour=rtime.RTC_Hours;
    else
      tstruct.tm_hour=12+rtime.RTC_Hours;     // adjust hours + 12 for PM

    tstruct.tm_min=rtime.RTC_Minutes;
    tstruct.tm_sec=rtime.RTC_Seconds;

    tstruct.tm_year=rdate.RTC_Year+100;       // years since 1900. 00+100=2000
    tstruct.tm_mon=rdate.RTC_Month-1;
    tstruct.tm_mday=rdate.RTC_Date;

    return mktime(&tstruct)-946684800;        // subtract our base
  }


  /**
   * Set the timer from the tick value. Assumes that 00 year = 2000
   * Not IRQ safe due to use of gmtime()
   * @param tick The time_t value
   */

  inline void RtcBase::setTick(uint32_t tick) const {

    struct tm *tstruct;

    tick+=946684800;            // tick zero = 1/1/2000 00:00:00
    tstruct=gmtime(reinterpret_cast<time_t *>(&tick));

    // date is straightforward

    setDate(tstruct->tm_year-100,tstruct->tm_mon+1,tstruct->tm_mday,tstruct->tm_wday==0 ? 7 : tstruct->tm_wday);

    // if in 24 hours then easy else the hour must be mod 12 and am/pm set

    if(_hourFormat==RTC_HourFormat_24)
      setTime(tstruct->tm_hour,tstruct->tm_min,tstruct->tm_sec,0);
    else
      setTime(tstruct->tm_hour % 12,tstruct->tm_min,tstruct->tm_sec,tstruct->tm_hour>=12 ? RTC_H12_PM : RTC_H12_AM);
  }
}
