/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB CDC header functional descriptor. See specification document:
     * "Universal Serial Bus Class Definitions for Communications Devices"
     */

    struct CdcHeaderFunctionalDescriptor : CdcFunctionalDescriptor {

      uint16_t bcdCDC;

      CdcHeaderFunctionalDescriptor()
        : CdcFunctionalDescriptor(CdcFunctionalDescriptor::HEADER,sizeof(CdcHeaderFunctionalDescriptor)),
          bcdCDC(0x0110) {      // version 1.10

        static_assert(sizeof(CdcHeaderFunctionalDescriptor)==5,"Compiler error: sizeof(CdcHeaderFunctionalDescriptor)!=5");
      }
    } __attribute__((packed));
  }
}
