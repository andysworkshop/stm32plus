/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * Template class that defines the Atmel AT24C32/64 interface.
   *
   * The Atmel AT24C32/64 is a 32768/65536 kbit serial EEPROM IC with an I2C
   * interface. Data can be read or written using single-byte random access or it can
   * be accessed serially in multiples of up to 32 bytes (a page). That is, you may
   * access from where you start up to the end of a page in one burst and then you must
   * update your position to the next page and may then read/write up to another 32 bytes
   * in a burst. This class takes care of all that internal stuff for you.
   *
   * @tparam TI2C The I2C type that you are going to use to communicate with this EEPROM
   */

  template<class TI2C,int TSizeInBytes>
  class AT24Cxx : public TI2C,
                  public SerialEeprom<AT24Cxx<TI2C,TSizeInBytes> > {

    public:
      enum {
        SIZE_IN_BYTES = TSizeInBytes,   ///< 32kbit/64Kbit
        SLAVE_ADDRESS = 0xa0            ///< I2C bus address
      };

    public:
      AT24Cxx(typename TI2C::Parameters& params);

      // methods to support SerialEeprom

      bool writeByte(uint8_t c);
      bool writeBytes(const uint8_t *buffer,uint32_t count);

      bool readByte(uint8_t& c);
      bool readBytes(uint8_t *buffer,uint32_t count);
  };


  /**
   * Derivation for the AT24C32
   * @tparam The I2C configuration that we use
   */

  template<class TI2C> using AT24C32=AT24Cxx<TI2C,4096>;


  /**
   * Derivation for the AT24C64
   * @tparam The I2C configuration that we use
   */

  template<class TI2C> using AT24C64=AT24Cxx<TI2C,8192>;


  /**
   * Constructor. Ensures that the incoming parameters are correct.
   * @param[in] params The parameters class that holds the I2C configuration
   */

  template<class TI2C,int TSizeInBytes>
  inline AT24Cxx<TI2C,TSizeInBytes>::AT24Cxx(typename TI2C::Parameters& params)
    : TI2C(params),
      SerialEeprom<AT24Cxx<TI2C,TSizeInBytes> >(*this) {

    // set the I2C slave address

    this->setSlaveAddress(0xa0);

    // this device has 2-byte addresses

    params.i2c_addressSize=2;
  }


  /**
   * Write a single byte to the device
   * @param c The byte to write
   * @return true if it worked
   */

  template<class TI2C,int TSizeInBytes>
  inline bool AT24Cxx<TI2C,TSizeInBytes>::writeByte(uint8_t c) {

    if(!TI2C::writeBytes(this->_position,&c,1))
      return false;

    this->_position++;
    return true;
  }


  /**
   * Write multiple bytes to the device. We take advantage of the ability to write
   * multiple bytes in one go when those bytes are all in one page.
   * @param[in] buffer The source of data to write
   * @param[in] count The number of bytes to write
   * @return true if it worked
   */

  template<class TI2C,int TSizeInBytes>
  inline bool AT24Cxx<TI2C,TSizeInBytes>::writeBytes(const uint8_t *buffer,uint32_t count) {

    uint32_t toWrite;
    const uint8_t *ptr;

    for(ptr=buffer;count;count-=toWrite) {

      toWrite=std::min<uint8_t>(count,0x20-(this->_position & 0x1f));
      if(!TI2C::writeBytes(this->_position,ptr,toWrite))
        return false;

      ptr+=toWrite;
      this->_position+=toWrite;

      if(count>0)
        MillisecondTimer::delay(10);
    }

    return true;
  }


  /**
   * Read a single byte from the device
   * @param[out] c A reference to the byte to read
   * @return true if it worked
   */

  template<class TI2C,int TSizeInBytes>
  inline bool AT24Cxx<TI2C,TSizeInBytes>::readByte(uint8_t& c) {

    if(!TI2C::readBytes(this->_position,&c,1))
      return false;

    this->_position++;
    return true;
  }


  /**
   * Read multiple bytes from the device. We take advantage of the ability to read
   * out multiple bytes in one go when those bytes are all in one page.
   * @param[out] buffer Where to read the data to
   * @param[in] count The number of bytes to read
   */

  template<class TI2C,int TSizeInBytes>
  inline bool AT24Cxx<TI2C,TSizeInBytes>::readBytes(uint8_t *buffer,uint32_t count) {

    uint32_t toRead;
    uint8_t *ptr;

    for(ptr=buffer;count;count-=toRead) {

      toRead=std::min<uint8_t>(count,0x20-(this->_position & 0x1f));
      if(!TI2C::readBytes(this->_position,ptr,toRead))
        return false;

      ptr+=toRead;
      this->_position+=toRead;
    }

    return true;
  }
}
