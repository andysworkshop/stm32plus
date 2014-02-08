/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"
#include "config/i2c.h"


namespace stm32plus {


  /**
   * Reset one or more bits
   * @param address The register address
   * @param mask The bitmask to clear
   * @return true if it worked
   */

  bool I2CSingleByteMasterPollingFeature::resetBits(uint8_t address,uint8_t mask) const {

    uint8_t value;

    // read current register value

    if(!readByte(address,value))
      return false;

    // clear the bits

    value&=~mask;

    // write back to the register

    return writeByte(address,value);
  }


  /**
   * Set one or more bits
   * @param address The register address
   * @param mask The bitmask to set
   * @return true if it worked
   */

  bool I2CSingleByteMasterPollingFeature::setBits(uint8_t address,uint8_t mask) const {

    uint8_t value;

    // read current register value

    if(!readByte(address,value))
      return false;

    // set the bits

    value|=mask;

    // write back to the register

    return writeByte(address,value);
  }


  /**
   * Set a value by merging into the current value. The bits defined by the mask are the
   * ones that get set by this call. The bits that are zero in the mask are preserved.
   * @param address The register address
   * @param value The value to mask in
   * @param mask The mask that defines the values to set
   * @return true if it worked
   */

  bool I2CSingleByteMasterPollingFeature::setMask(uint8_t address,uint8_t newbits,uint8_t mask) const {

    uint8_t value;

    // read current register value

    if(!readByte(address,value))
      return false;

    // set the bits

    value&=~mask;
    value|=newbits;

    // write back to the register

    return writeByte(address,value);
  }
}
