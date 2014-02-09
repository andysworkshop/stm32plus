/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)

#include "config/i2c.h"


namespace stm32plus {


  /**
   * Read some bytes from the slave.
   * @param address The first byte of the address
   * @param count How many bytes to read
   * @param output Where to put the bytes
   * @return false if there's an error, true otherwise
   */

  bool I2CMasterPollingFeature::readBytes(const uint8_t *address,uint8_t *output,uint32_t count) const {

    // do the protocol up to the actual data read

    if(!prepareRead(address))
      return false;

    // loop reading the bytes from the slave

    while(count--) {

      // an ACK is required in between each byte except the last.
      // the last byte gets a NACK after it and before the STOP

      if(!count) {

        __disable_irq();
        I2C_AcknowledgeConfig(_i2c,DISABLE);
        I2C_GenerateSTOP(_i2c,ENABLE);
        __enable_irq();
      }

      if(!checkEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        return false;

      // read the data byte

      *output++=I2C_ReceiveData(_i2c);
    }

    return true;
  }


  /**
   * Prepare for a read from the slave. This will do the I2C protocol up to the point of sending the
   * address bytes to read from. After the successful completion of this function you are expected to complete
   * the read either by polling or by DMA using the I2CDmaReaderFeature class.
   * @param address The I2C address
   * @return true if it worked
   */

  bool I2CMasterPollingFeature::prepareRead(const uint8_t *address) const {

    uint8_t count;

    // enable ACKs for the reads

    I2C_AcknowledgeConfig(_i2c,ENABLE);

    // generate the start condition

    I2C_GenerateSTART(_i2c,ENABLE);

    // Test on I2C EV5 and clear it

    if(!checkEvent(I2C_EVENT_MASTER_MODE_SELECT))
      return false;

    // send the slave address

    I2C_Send7bitAddress(_i2c,_slaveAddress,I2C_Direction_Transmitter);

    // Test on I2C EV6 and clear it

    if(!checkEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
      return false;

    // send the address

    for(count=_i2c.getAddressSize();count;count--) {

      I2C_SendData(_i2c,*address++);

      // Test on I2C EV8 and clear it

      if(!checkEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        return false;
    }

    // clear AF flag if arised */

    ((I2C_TypeDef *)_i2c)->SR1=0x0400;

    // generate the start condition again

    I2C_GenerateSTART(_i2c,ENABLE);

    // Test on I2C EV6 and clear it

    if(!checkEvent(I2C_EVENT_MASTER_MODE_SELECT))
      return false;

    // send the slave address

    I2C_Send7bitAddress(_i2c,_slaveAddress,I2C_Direction_Receiver);

    // Test on I2C EV6 and clear it

    return checkEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
  }


  /**
   * Write some bytes to the slave
   * @param address The register address
   * @param count The number of bytes to write
   * @param input The source of data to write
   * @return false if it fails, true if it succeeds
   */

  bool I2CMasterPollingFeature::writeBytes(const uint8_t *address,const uint8_t *input,uint32_t count) const {

    // do the protocol up to and including the address transfer

    if(!prepareWrite(address))
      return false;

    // now start transferring the data

    while(count--) {

      // send data

      I2C_SendData(_i2c,*input++);

      // test on I2C EV8 and clear it

      if(!checkEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        return false;
    }

    // send STOP condition

    I2C_GenerateSTOP(_i2c,ENABLE);
    return true;
  }


  /**
   * Prepare for a write to the peripheral. The I2C protocol is followed up to the point of
   * sending the address but not the data. After the successful return of this function you are
   * expected to initiate the data transfer either by polling write or by DMA write using
   * the I2CDmaWriterFeature class.
   * @param address The I2C address to prepare writing to
   * @return true if it worked
   */

  bool I2CMasterPollingFeature::prepareWrite(const uint8_t *address) const {

    uint8_t count;

    // generate the start condition

    I2C_GenerateSTART(_i2c,ENABLE);

    // Test on I2C EV5 and clear it

    if(!checkEvent(I2C_EVENT_MASTER_MODE_SELECT))
      return false;

    // send the slave address

    I2C_Send7bitAddress(_i2c,_slaveAddress,I2C_Direction_Transmitter);

    // Test on I2C EV6 and clear it */

    if(!checkEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
      return false;

    for(count=_i2c.getAddressSize();count;count--) {

      // send the address byte

      I2C_SendData(_i2c,*address++);

      // Test on I2C EV8 and clear it

      if(!checkEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        return false;
    }

    return true;
  }


  /**
   * Check that an event has occurred, or timeout
   * @param eventId The event to check
   * @return false if it fails
   */

  bool I2CMasterPollingFeature::checkEvent(uint32_t eventId) const {

    uint32_t timeoutStart;

    // get the time now

    timeoutStart=MillisecondTimer::millis();

    // wait for the event to be raised or timeout his hit (should take care of timeout wrap here)

    while(!I2C_CheckEvent(_i2c,eventId)) {
      if(MillisecondTimer::millis()-timeoutStart>_timeout)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);
    }

    return true;
  }
}


#endif
