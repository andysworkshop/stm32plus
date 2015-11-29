/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB CDC union functional descriptor. See specification document:
     * "Universal Serial Bus Class Definitions for Communications Devices"
     */

    struct CdcOneSlaveUnionFunctionalDescriptor : CdcFunctionalDescriptor {

      uint8_t bMasterInterface;
      uint8_t bSlaveInterface;

      CdcOneSlaveUnionFunctionalDescriptor()
        : CdcFunctionalDescriptor(CdcFunctionalDescriptor::UNION,sizeof(CdcOneSlaveUnionFunctionalDescriptor)) {

        static_assert(sizeof(CdcOneSlaveUnionFunctionalDescriptor)==5,"Compiler error: sizeof(CdcOneSlaveUnionFunctionalDescriptor)!=5");
      }
    } __attribute__((packed));
  }
}
