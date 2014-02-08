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
   * RTC feature to enable access to the alarm functionality. The F4 has two independent alarms
   * that can be configured to trigger on any or all of the time components and optionally the date/weekday.
   */

  template<uint32_t TAlarmIndex>
  class RtcAlarmInterruptFeature : public RtcFeatureBase,
                                   public ExtiRtcAlarm {

    public:
      RtcAlarmInterruptFeature(RtcBase& rtc);

      void setAlarm(uint32_t mask,uint32_t dateOrWeekdaySelector,uint8_t dateOrWeekDayValue,uint8_t hour,uint8_t minute,uint8_t second,uint8_t am_pm=RTC_H12_AM) const;
      void cancelAlarm() const;
  };


  /**
   * Typedefs for both the possible alarms
   */

  typedef RtcAlarmInterruptFeature<RTC_Alarm_A> RtcAlarmAInterruptFeature;
  typedef RtcAlarmInterruptFeature<RTC_Alarm_B> RtcAlarmBInterruptFeature;


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  template<uint32_t TAlarmIndex>
  inline RtcAlarmInterruptFeature<TAlarmIndex>::RtcAlarmInterruptFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc),
      ExtiRtcAlarm(EXTI_Mode_Interrupt,EXTI_Trigger_Rising) {
  }


  /**
   * Set the alarm
   * @param mask one of RTC_AlarmMask_Definitions, e.g. RTC_AlarmMask_All
   * @param dateOrWeekdaySelector one of RTC_AlarmDateWeekDay_Definitions. e.g. RTC_AlarmDateWeekDaySel_Date (don't care if date is masked off)
   * @param hour The hour, if not masked off
   * @param minute The minute, if not masked off
   * @param second The second, if not masked off
   * @param dateOrWeekDayValue The date (1..31) or day (1..7) where Monday = 1, if not masked off.
   */

  template<uint32_t TAlarmIndex>
  inline void RtcAlarmInterruptFeature<TAlarmIndex>::setAlarm(uint32_t mask,
                                                              uint32_t dateOrWeekdaySelector,
                                                              uint8_t dateOrWeekDayValue,
                                                              uint8_t hour,
                                                              uint8_t minute,
                                                              uint8_t second,
                                                              uint8_t am_pm) const {

    RTC_AlarmTypeDef alarm;

    // alarm register is RO while enabled

    RTC_AlarmCmd(TAlarmIndex,DISABLE);

    // set up the time component

    alarm.RTC_AlarmTime.RTC_Hours=hour;
    alarm.RTC_AlarmTime.RTC_Minutes=minute;
    alarm.RTC_AlarmTime.RTC_Seconds=second;
    alarm.RTC_AlarmTime.RTC_H12=am_pm;

    // set up the alarm configuration component

    alarm.RTC_AlarmMask=mask;
    alarm.RTC_AlarmDateWeekDaySel=dateOrWeekdaySelector;
    alarm.RTC_AlarmDateWeekDay=dateOrWeekDayValue;

    // set the alarm and enable the interrupt

    RTC_SetAlarm(RTC_Format_BIN,TAlarmIndex,&alarm);
    RTC_ITConfig(TAlarmIndex==RTC_Alarm_A ? RTC_IT_ALRA : RTC_IT_ALRB,ENABLE);

    // enable the alarm

    RTC_AlarmCmd(TAlarmIndex,ENABLE);
  }


  /**
   * Cancel the alarm interupts
   */

  template<uint32_t TAlarmIndex>
  inline void RtcAlarmInterruptFeature<TAlarmIndex>::cancelAlarm() const {
    RTC_AlarmCmd(TAlarmIndex,DISABLE);
    RTC_ITConfig(TAlarmIndex==RTC_Alarm_A ? RTC_IT_ALRA : RTC_IT_ALRB,DISABLE);
  }
}
