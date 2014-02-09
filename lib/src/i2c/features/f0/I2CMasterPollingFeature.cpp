/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"

#if defined(STM32PLUS_F0)

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

    uint32_t blocks,startStopMode;
    uint8_t readCount,remainder;
    bool first;

    // send address

    if(!prepareRead(address))
      return false;

    // get number of reload cycles

    blocks=count/255;
    remainder=count % 255;

    if(!checkEvent(I2C_ISR_TC))
      return false;

    // process whole number of 255 byte transfers

    if(blocks>0) {

      first=true;

      // wait until all reload cycles are performed

      while(blocks!=0) {

        if(!first && !checkEvent(I2C_ISR_TCR))
          return false;

        if(blocks==1 && remainder==0)   // last whole block
          I2C_TransferHandling(_i2c,_slaveAddress,255,I2C_AutoEnd_Mode,first ? I2C_Generate_Start_Read : I2C_No_StartStop);
        else                            // whole block, not last
          I2C_TransferHandling(_i2c, _slaveAddress,255,I2C_Reload_Mode,first ? I2C_Generate_Start_Read : I2C_No_StartStop);

        first=false;
        readCount=0;

        while(readCount!=255) {

          if(!checkEvent(I2C_ISR_RXNE))
            return false;

          // get a byte

          *output++=I2C_ReceiveData(_i2c);

          readCount++;
        }

        // processed a 255 byte block

        blocks--;
      }

      // set up to process the remainder

      if(remainder>0) {

        startStopMode=I2C_No_StartStop;

        if(!checkEvent(I2C_ISR_TCR))
          return false;
      }
    }
    else
      startStopMode=I2C_Generate_Start_Read;

    // remaining transfer

    if(remainder>0) {

      I2C_TransferHandling(_i2c,_slaveAddress,remainder,I2C_AutoEnd_Mode,startStopMode);

      while(remainder--) {

        // wait for RXNE flag

        if(!checkEvent(I2C_ISR_RXNE))
          return false;

        *output++=I2C_ReceiveData(_i2c);
      }
    }

    // wait for STOPF

    if(!checkEvent(I2C_ISR_STOPF))
      return false;

    // clear STOPF

    I2C_ClearFlag(_i2c,I2C_ICR_STOPCF);
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

    I2C_TransferHandling(_i2c,_slaveAddress,_i2c.getAddressSize(),I2C_SoftEnd_Mode,I2C_Generate_Start_Write);

    // send the address

    for(count=_i2c.getAddressSize();count;count--) {

      // wait until TXIS flag is set

      if(!checkEvent(I2C_ISR_TXIS))
        return false;

      I2C_SendData(_i2c,*address++);
    }

    return true;
  }


  /**
   * Write some bytes to the slave
   * @param address The register address
   * @param count The number of bytes to write
   * @param input The source of data to write
   * @return false if it fails, true if it succeeds
   */

  bool I2CMasterPollingFeature::writeBytes(const uint8_t *address,const uint8_t *input,uint32_t count) const {

    // send address

    if(!prepareWrite(address))
      return false;

    // wait for TCR

    if(!checkEvent(I2C_ISR_TCR))
      return false;

    I2C_TransferHandling(_i2c,_slaveAddress,count,I2C_AutoEnd_Mode,I2C_No_StartStop);

    while(count--) {

      // wait for TXIS

      if(!checkEvent(I2C_ISR_TXIS))
        return false;

      // write data to TXDR

      I2C_SendData(_i2c,*input++);
    }

    // wait for STOPF

    if(!checkEvent(I2C_ISR_STOPF))
      return false;

    // clear STOPF

    I2C_ClearFlag(_i2c,I2C_ICR_STOPCF);
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

    I2C_TransferHandling(_i2c,_slaveAddress,_i2c.getAddressSize(),I2C_Reload_Mode,I2C_Generate_Start_Write);

    // send the address

    for(count=_i2c.getAddressSize();count;count--) {

      // wait until TXIS flag is set

      if(!checkEvent(I2C_ISR_TXIS))
        return false;

      I2C_SendData(_i2c,*address++);
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

    while(I2C_GetFlagStatus(_i2c,eventId)==0)
      if(MillisecondTimer::millis()-timeoutStart>_timeout)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2C,I2C::E_I2C_TIMEOUT);

    return true;
  }
}


#endif
