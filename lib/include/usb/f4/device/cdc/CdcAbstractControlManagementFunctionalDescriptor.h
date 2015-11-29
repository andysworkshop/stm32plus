/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB CDC abstract control management functional descriptor. See specification document:
     * "Universal Serial Bus Class Definitions for Communications Devices"
     */

    struct CdcAbstractControlManagementFunctionalDescriptor : CdcFunctionalDescriptor {

      uint8_t bmCapabilities;

      CdcAbstractControlManagementFunctionalDescriptor()
        : CdcFunctionalDescriptor(CdcFunctionalDescriptor::ABSTRACT_CONTROL_MANAGEMENT,sizeof(CdcAbstractControlManagementFunctionalDescriptor)) {

        static_assert(sizeof(CdcAbstractControlManagementFunctionalDescriptor)==4,"Compiler error: sizeof(CdcAbstractControlManagementFunctionalDescriptor)!=4");
      }
    } __attribute__((packed));
  }
}
