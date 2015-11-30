/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0_42
#error This class can only be used with the STM32F042 series
#endif


namespace stm32plus {

  /*
   * A single entry in the BDT. These must strictly be 16-bit fields
   */

  struct UsbBufferDescriptionTableEntry {

    volatile uint16_t addr;
    volatile uint16_t count;

    /*
     * Get just the count from the register
     */

    uint16_t getCount() volatile {
      return count & 0x3ff;
    }


    /*
     * Set the encoded RX count
     */

    void setRxCount(int length) volatile {

      uint16_t wNBlocks;

      if((length)>62) {

        wNBlocks = length >> 5;
        if((length & 0x1f)==0)
          wNBlocks--;

        count=(wNBlocks << 10) | 0x8000;
      }
      else {

        wNBlocks=length >> 1;

        if((length & 0x1)!=0)
          wNBlocks++;

        count=wNBlocks << 10;
      }
    }
  } __attribute__((packed));
}
