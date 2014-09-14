/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if !defined(STM32PLUS_F1) && !defined(STM32PLUS_F4)
#error Only F1 and F4 supported
#endif


namespace stm32plus {


  /**
   * The slave feature class provides access to operations applicable to an I2C slave
   */

  class I2CSlaveFeature : public I2CIoFeature {

    public:
      I2CSlaveFeature(I2C& i2c);

      bool prepareRead() const;
      bool readBytes(void *buffer,uint32_t count,uint32_t& actuallyRead) const;
      bool finishRead() const;

      bool prepareWrite() const;
      bool writeBytes(const void *buffer,uint32_t count,uint32_t& actuallyWritten) const;
  };


  /**
   * Constructor
   * @param i2c The i2c feature base reference
   */

  inline I2CSlaveFeature::I2CSlaveFeature(I2C& i2c)
    : I2CIoFeature(i2c) {
  }


  /**
   * Prepare for reading by blocking until the slave address is detected on the bus or the timeout
   * expires. When this method has successfully returned then you can call readBytes() as many times
   * as necessary to read the data stream followed by finishRead() to ensure that the master has
   * sent the STOP condition.
   * @return true if it works, false if there's a timeout
   */

  inline bool I2CSlaveFeature::prepareRead() const {

    uint32_t timeoutStart;

    // wait until the slave address (or the optional second address) is matched on the bus (EV1)

    timeoutStart=MillisecondTimer::millis();

    while(!I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) && !I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED))
      if(MillisecondTimer::millis()-timeoutStart>_timeout)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);

    // the slave address has been matched on the bus

    return true;
  }


  /**
   * Synchronously read some bytes from the master, aborting after the timeout has expired. This method
   * will read up to 'count' bytes from the master. If the master sends the STOP condition or there is
   * an error then 'actuallyRead' will be less than 'count'. Receipt of the STOP condition does not
   * result in an error.
   * @param buffer Where to read the bytes to
   * @param count The number of bytes to read
   * @return true if it works, false if there's a timeout
   */

  inline bool I2CSlaveFeature::readBytes(void *buffer,uint32_t count,uint32_t& actuallyRead) const {

    uint32_t timeoutStart;
    uint8_t *ptr;

    // reset the read count and get a byte pointer

    actuallyRead=0;
    ptr=reinterpret_cast<uint8_t *>(buffer);

    // now we're expecting a stream of bytes

    timeoutStart=MillisecondTimer::millis();

    while(count) {

      if(I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_BYTE_RECEIVED)) {

        // data ready to read

        *ptr++=I2C_ReceiveData(_i2c);
        actuallyRead++;
        count--;

        // reset the timeout for the next byte

        timeoutStart=MillisecondTimer::millis();
      }
      else if(I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_STOP_DETECTED)) {    // EOF from the master
        I2C_ClearFlag(_i2c,I2C_FLAG_STOPF);
        break;
      }

      // check for timeout

      if(MillisecondTimer::millis()-timeoutStart>_timeout)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);
    }

    // all done

    return true;
  }


  /**
   * When readReadBytes() has been used to read the exact full number of bytes from the master
   * (actuallyRead==count==data transmitted by master) then finishRead() is required to be called
   * so that the STOP condition is read.
   * @return true if it worked, false if there's a timeout
   */

  inline bool I2CSlaveFeature::finishRead() const {

    uint32_t timeoutStart;

    // wait until the slave address (or the optional second address) is matched on the bus (EV1)

    timeoutStart=MillisecondTimer::millis();

    while(!I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_STOP_DETECTED))
      if(MillisecondTimer::millis()-timeoutStart>_timeout)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);

    // all OK

    I2C_ClearFlag(_i2c,I2C_FLAG_STOPF);
    return true;
  }


  /**
   * Wait until the slave address (TX mode) is matched on the bus or a timeout occurs. When this method
   * returns you can call writeBytes() to write out as many bytes as required.
   * @return true if it worked, false if there was a timeout
   */

  inline bool I2CSlaveFeature::prepareWrite() const {

    uint32_t timeoutStart;

    // wait until the slave address (or the optional second address) is matched on the bus (EV1)

    timeoutStart=MillisecondTimer::millis();

    while(!I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) && !I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED))
      if(MillisecondTimer::millis()-timeoutStart>_timeout)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);

    // the slave address has been matched on the bus

    return true;
  }


  /**
   * Write a
   * @param buffer
   * @param count
   * @param actuallyRead
   * @return
   */

  inline bool I2CSlaveFeature::writeBytes(const void *buffer,uint32_t count,uint32_t& actuallyWritten) const {

    uint32_t timeoutStart;
    const uint8_t *ptr;

    // reset the read count and get a byte pointer

    actuallyWritten=0;
    ptr=reinterpret_cast<const uint8_t *>(buffer);
    timeoutStart=MillisecondTimer::millis();

    while(count) {

      // check for NACK (master telling us to stop sending data)

      if(I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_ACK_FAILURE))
        break;

      // write a byte

      I2C_SendData(_i2c,*ptr);

      // wait for TXE or NACK

      while(!I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_BYTE_TRANSMITTED)) {

        // NACK is possible here

        if(I2C_CheckEvent(_i2c,I2C_EVENT_SLAVE_ACK_FAILURE))
          goto end;

        // check for timeout

        if(MillisecondTimer::millis()-timeoutStart>_timeout)
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);
      }

      // byte sent, reset timeout

      timeoutStart=MillisecondTimer::millis();
    }

  end:
    return true;
  }
}
