/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Interface association descriptor (interface grouping)
     */

    struct InterfaceAssociationDescriptor {

      DescriptorHeader header;

      uint8_t bFirstInterface;
      uint8_t bInterfaceCount;
      uint8_t bFunctionClass;
      uint8_t bFunctionSubClass;
      uint8_t bFunctionProtocol;
      uint8_t iFunction;

      /**
       * Constructor - set up the constants
       */

      InterfaceAssociationDescriptor()
        : header(8,0xb) {

        static_assert(sizeof(InterfaceAssociationDescriptor)==8,"Compilation error: sizeof(InterfaceAssociationDescriptor)!=8");
      }
    } __attribute((packed));
  }
}
