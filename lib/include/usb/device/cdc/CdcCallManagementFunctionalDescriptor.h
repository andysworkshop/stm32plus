/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB CDC call management functional descriptor. See specification document:
     * "Universal Serial Bus Class Definitions for Communications Devices"
     */

    struct CdcCallManagementFunctionalDescriptor : CdcFunctionalDescriptor {

      uint8_t bmCapabilities;
      uint8_t bDataInterface;

      CdcCallManagementFunctionalDescriptor()
        : CdcFunctionalDescriptor(CdcFunctionalDescriptor::CALL_MANAGEMENT,sizeof(CdcCallManagementFunctionalDescriptor)) {

        static_assert(sizeof(CdcCallManagementFunctionalDescriptor)==5,"Compiler error: sizeof(CdcCallManagementFunctionalDescriptor)!=5");
      }
    } __attribute__((packed));
  }
}
