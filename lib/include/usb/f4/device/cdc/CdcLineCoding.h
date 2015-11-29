/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * CDC line coding structure
     */

    struct CdcLineCoding {

      uint32_t dwDTERate;             // bits/second
      uint8_t  bCharFormat;
      uint8_t  bParityType;
      uint8_t  bDataBits;             // 5/6/7/8/16

      enum {
        CHARFORMAT_1_STOP_BIT = 0,
        CHARFORMAT_1_5_STOP_BITS = 1,
        CHARFORMAT_2_STOP_BITS = 2
      };

      enum {
        PARITY_NONE  = 0,
        PARITY_ODD   = 1,
        PARITY_EVEN  = 2,
        PARITY_MARK  = 3,
        PARITY_SPACE = 4
      };

      CdcLineCoding() {

        static_assert(sizeof(CdcLineCoding)==7,"Compiler error: sizeof(LineCoding)!=7");

        // set a default of 9600/8/N/1

        dwDTERate=9600;
        bCharFormat=CHARFORMAT_1_STOP_BIT;
        bParityType=PARITY_NONE;
        bDataBits=8;
      }

    } __attribute__((packed));
  }
}
