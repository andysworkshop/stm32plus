/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {


  /**
   * The DS1307 is a real time clock from Maxim IC. The interface to it is I2C. 12 and 24 hour
   * clock modes are supported as are all the 'non-RTC' functions that the DS1307 gives us
   * as a sort of bonus:
   *
   *   square wave output: 1Hz, 4kHz, 8kHz or 32kHz
   *   56 bytes of battery backed up RAM
   *
   * This class is parameterised by the I2CPeripheral instance that you want to use to talk to
   * the device. For example, "DS1307<I2C1_Default> i2c(params)" would configure the DS1307
   * to use I2C peripheral #1.
   *
   * The DS1307 talks BCD. Humans don't. We talk numbers, and these are the valid ranges for
   * the date components:
   *
   *   seconds:      0-59
   *   minutes:      0-59
   *   hours:        1-12am/pm or 0-23
   *   day-of-week:  1-7 where 1=Sunday, 2=Monday
   *   day-of-month: 1-31
   *   month:        1-12
   *   year:         0-99 (interpret the century as you will)
   */

  template<class TI2CPeripheral>
  class DS1307 : public TI2CPeripheral {

    public:

      /**
       * Parameters class that inherits from the I2C parameters
       * and adds any custom parameters for the DS1307. At present
       * there are none.
       */

      struct Parameters : public TI2CPeripheral::Parameters {
      };


      /**
       * The I2C bus address for the peripheral
       */

      enum {
        I2C_BUS_ADDRESS = 0xD0    //!< 11010000
      };


      /**
       * The possible square wave output frequencies
       */

      enum SquareWaveFrequency {
        SQW_1HZ     = 0,  //!< SQW_1HZ
        SQW_4096HZ  = 1,  //!< SQW_4096HZ
        SQW_8192HZ  = 2,  //!< SQW_8192HZ
        SQW_32768HZ = 3   //!< SQW_32768HZ
      };


      /**
       * The command set for the DS1307
       */

      enum Command {
        REG_SECOND  = 0,    //!< REG_SECOND
        REG_MINUTE  = 1,    //!< REG_MINUTE
        REG_HOUR    = 2,    //!< REG_HOUR
        REG_DAY     = 3,    //!< REG_DAY
        REG_DATE    = 4,    //!< REG_DATE
        REG_MONTH   = 5,    //!< REG_MONTH
        REG_YEAR    = 6,    //!< REG_YEAR
        REG_CONTROL = 7,    //!< REG_CONTROL
        REG_RAM_0   = 8     //!< REG_RAM_0
      };

    protected:
      uint8_t toBcd(uint8_t binary) const;
      uint8_t fromBcd(uint8_t bcd) const;

    public:

      DS1307(const Parameters& params);

      bool enableSquareWaveOutput() const;
      bool disableSquareWaveOutput() const;

      bool setSquareWaveFrequency(SquareWaveFrequency frequency) const;
      bool setDisabledSquareWavePinOutputLevel(bool level) const;

      bool set24HourMode() const;
      bool set12HourMode() const;

      bool getHourMode(bool& hourMode12) const;

      bool startClock() const;
      bool stopClock() const;

      bool setDateTime(time_t dateTime) const;
      bool setDateTime(uint8_t year,uint8_t month,uint8_t monthDay,uint8_t weekDay,uint8_t hour,uint8_t minute,uint8_t second) const;

      bool setYear(uint8_t y) const;
      bool setMonth(uint8_t m) const;
      bool setMonthDay(uint8_t md) const;
      bool setWeekDay(uint8_t wd) const;

      bool setHour(uint8_t h) const;
      bool setMinute(uint8_t m) const;
      bool setSecond(uint8_t s) const;

      bool getDateTime(time_t& t) const;
      bool getDateTime(uint8_t& year,uint8_t& month,uint8_t& monthDay,uint8_t& weekDay,uint8_t& hour,uint8_t& minute,uint8_t& second) const;

      bool writeRamAddress(uint8_t address,uint8_t value) const;
      bool readRamAddress(uint8_t address,uint8_t& value) const;
  };


  /**
   * Constructor
   * @param params The parameters class used to initialise
   */

  template<class TI2CPeripheral>
  inline DS1307<TI2CPeripheral>::DS1307(const Parameters& params)
    : TI2CPeripheral(params) {

    // set our slave address. we're always a slave so there has to be an I/O feature
    // somewhere in the feature hierarchy that has this method

    this->setSlaveAddress(I2C_BUS_ADDRESS);
  }


  /**
   * Enable the square wave output. Set control bit 4 to 1
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::enableSquareWaveOutput() const {
    return this->setBits(REG_CONTROL,0x16);
  }


  /**
   * Disable the square wave output. Set control bit 4 to 0
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::disableSquareWaveOutput() const {
    return this->resetBits(REG_CONTROL,0x16);
  }


  /**
   * Set the frequency of the square wave output
   * @param frequency from the SquareWaveFrequency enumeration.
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setSquareWaveFrequency(SquareWaveFrequency frequency) const {
    return this->setMask(REG_CONTROL,frequency,0x3);
  }


  /**
   * Set the output level for the SQW/OUT when the square wave is not used
   * @param level false=LOW, true=HIGH
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setDisabledSquareWavePinOutputLevel(bool level) const {
    return level ? this->setBits(REG_CONTROL,0x80) : this->resetBits(REG_CONTROL,0x80);
  }


  /**
   * Set the device into 24 hour mode. Clear bit 6 of the hours register.
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::set24HourMode() const {
    return this->resetBits(REG_HOUR,0x40);
  }


  /**
   * Set the device into 12 hour mode. Set bit 6 of the hours register.
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::set12HourMode() const {
    return this->setBits(REG_HOUR,0x40);
  }


  /**
   * Start the clock ticker. Clear bit 7 (CH) of the seconds register.
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::startClock() const {
    return this->resetBits(REG_SECOND,0x80);
  }


  /**
   * Stop the clock ticker. Set bit 7 (CH) of the seconds register.
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::stopClock() const {
    return this->setBits(REG_SECOND,0x80);
  }


  /**
   * Write to the battery-backed RAM onboard the DS1307. There's 56 bytes of it (woohoo!) to
   * do with as you please.
   * @param address The RAM address index (0..55)
   * @param value The value to write
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::writeRamAddress(uint8_t address,uint8_t value) const {
    return this->writeByte(REG_RAM_0+address,value);
  }


  /**
   * Read from the battery-backed RAM onboard the DS1307.
   * @param address The RAM address index (0..55)
   * @param value The value successfully read (or undefined if it fails)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::readRamAddress(uint8_t address,uint8_t& value) const {
    return this->readByte(REG_RAM_0+address,value);
  }


  /**
   * Set the date and time from a libc time_t 32-bit number of seconds.
   * See http://www.cplusplus.com/reference/ctime/time_t. The two digit limitation on the clock year
   * means that the century component is lost. There is no consideration given to timezones.
   * @param dateTime The date and time from which to set the clock
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setDateTime(time_t dateTime) const {

    struct tm *tt;
    // get the time components

    tt=::gmtime(&dateTime);

    return
      setSecond(tt->tm_sec) &&        // do this first so we have 1-second to write the others
      setYear(tt->tm_year % 100) &&
      setMonth(tt->tm_mon+1) &&
      setMonthDay(tt->tm_mday) &&
      setWeekDay(tt->tm_wday+1) &&
      setHour(tt->tm_hour) &&
      setMinute(tt->tm_min);
  }


  /**
   * Convenience function to set all the date components in one go
   * @param year
   * @param month
   * @param monthDay
   * @param weekDay
   * @param hour
   * @param minute
   * @param second
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setDateTime(uint8_t year,uint8_t month,uint8_t monthDay,uint8_t weekDay,uint8_t hour,uint8_t minute,uint8_t second) const {

    return
      setYear(year) &&
      setMonth(month) &&
      setMonthDay(monthDay) &&
      setWeekDay(weekDay) &&
      setHour(hour) &&
      setMinute(minute) &&
      setSecond(second);
  }


  /**
   * Set the year component
   * @param y The year (0..99)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setYear(uint8_t y) const {
    return this->writeByte(REG_YEAR,toBcd(y));
  }


  /**
   * Set the month component
   * @param m The month (1..12)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setMonth(uint8_t m) const {
    return this->writeByte(REG_MONTH,toBcd(m));
  }

  /**
   * Set the day of month component
   * @param m The month date (1..31)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setMonthDay(uint8_t md) const {
    return this->writeByte(REG_DATE,toBcd(md));
  }

  /**
   * Set the day of week component
   * @param m The day of week (1..7 where 1=Sunday)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setWeekDay(uint8_t wd) const {
    return this->writeByte(REG_DAY,toBcd(wd));
  }


  /**
   * Get the 12/24 running mode
   * @param hourMode12 Set to true if 12 hour mode, false if 24 mode
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::getHourMode(bool& hourMode12) const {

    uint8_t value;

    if(!this->readByte(REG_HOUR,value))
      return false;

    hourMode12=(value & 0x40)!=0;
    return true;
  }


  /**
   * Set the hour component. The 12/24 hour mode of the peripheral is handled correctly.
   * @param h The hour (0..23)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setHour(uint8_t h) const {

    bool hourMode12;

    // get the 12/24 selector

    if(!getHourMode(hourMode12))
      return false;

    if(hourMode12) {

      // convert 0..23 to 1..12

      if((h=h % 12)==0)
        h=12;

      // write the new value

      if(!this->setMask(REG_HOUR,toBcd(h),0x1f))
        return false;
    }
    else {

      // just write out

      if(!this->setMask(REG_HOUR,toBcd(h),0x3f))
        return false;
    }

    return true;
  }


  /**
   * Set the minute component
   * @param m The minute (0..59)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setMinute(uint8_t m) const {
    return this->writeByte(REG_MINUTE,m);
  }


  /**
   * Set the second component
   * @param m The second (0..59)
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::setSecond(uint8_t s) const {
    return this->setMask(REG_SECOND,s,0x7f);
  }


  /**
   * Get the date/time as a time_t component. See See http://www.cplusplus.com/reference/ctime/time_t.
   * The 00..99 DS1307 year is assumed to be in the range 2000..2099
   * @param t Where to store the converted time
   * @return false if it fails
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::getDateTime(time_t& t) const {

    uint8_t year,month,monthDay,weekDay,hour,minute,second;
    struct tm tt;

    // get the components

    if(!getDateTime(year,month,monthDay,weekDay,hour,minute,second))
      return false;

    // do the conversion

    tt.tm_year=year+2000;
    tt.tm_mon=month-1;
    tt.tm_mday=month;
    tt.tm_wday=weekDay-1;
    tt.tm_hour=hour;
    tt.tm_min=minute;
    tt.tm_sec=second;

    t=mktime(&tt);
    return true;
  }


  /**
   * Get the date/time components
   * @param year (0..99)
   * @param month (1..12)
   * @param monthDay (1..31)
   * @param weekDay (1..7 where Sunday=1)
   * @param hour (0..23)
   * @param minute (0..59)
   * @param second (0..59)
   * @return false if it fails, and the return values are undefined.
   */

  template<class TI2CPeripheral>
  inline bool DS1307<TI2CPeripheral>::getDateTime(uint8_t& year,uint8_t& month,uint8_t& monthDay,uint8_t& weekDay,uint8_t& hour,uint8_t& minute,uint8_t& second) const {

    uint8_t values[7];

    // get them all in one read

    if(!this->readBytes(REG_SECOND,values,7))
      return false;

    // conversion

    year=fromBcd(values[REG_YEAR] & 0x7f);
    month=fromBcd(values[REG_YEAR] & 0x1f);
    monthDay=fromBcd(values[REG_DATE] & 0x3f);
    weekDay=fromBcd(values[REG_DAY] & 0x7);

    if((values[REG_HOUR] & 0x40)!=0) {

      // get the 1..12 value

      hour=fromBcd(values[REG_HOUR] & 0x1f);

      if((values[REG_HOUR] & 0x20)!=0)
        hour+=12;             // is PM
      else {
        if(hour==12)          // is AM where 12am == 00 hours
          hour=0;
      }
    }
    else
      hour=fromBcd(values[REG_HOUR] & 0x3f);

    minute=fromBcd(values[REG_MINUTE] & 0x7f);
    second=fromBcd(values[REG_SECOND] & 0x7f);

    return true;
  }


  /**
   * Convert value to 4-bit BCD
   * @param binary the binary value
   * @return The BCD value
   */

  template<class TI2CPeripheral>
  inline uint8_t DS1307<TI2CPeripheral>::toBcd(uint8_t binary) const {

    uint8_t bcd=0;

    while(binary!=0) {
      bcd<<=4;
      bcd|=binary % 10;
      binary/=10;
    }

    return bcd;
  }


  /**
   * Convert 4-bit BCD to binary
   * @param bcd The BCD value
   * @return The binary value
   */

  template<class TI2CPeripheral>
  inline uint8_t DS1307<TI2CPeripheral>::fromBcd(uint8_t bcd) const {

    uint8_t binary=0;

    while(bcd!=0) {
      binary*=10;
      binary|=bcd & 0xf;
      bcd>>=4;
    }

    return binary;
  }
}
