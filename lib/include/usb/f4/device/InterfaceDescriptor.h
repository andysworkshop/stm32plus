/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB interface descriptor
     */

    struct InterfaceDescriptor {

      DescriptorHeader header;

      uint8_t bInterfaceNumber;    // Number of Interface
      uint8_t bAlternateSetting;   // Value used to select alternative setting
      uint8_t bNumEndpoints;       // Number of Endpoints used for this interface
      uint8_t bInterfaceClass;     // Class Code (Assigned by USB Org)
      uint8_t bInterfaceSubClass;  // Subclass Code (Assigned by USB Org)
      uint8_t bInterfaceProtocol;  // Protocol Code (Assigned by USB Org)
      uint8_t iInterface;          // Index of String Descriptor Describing this interface

      InterfaceDescriptor()
        : header(sizeof(InterfaceDescriptor),USB_DESC_TYPE_INTERFACE) {

        static_assert(sizeof(InterfaceDescriptor)==9,"Compiler error: sizeof(InterfaceDescriptor)!=9");

        iInterface=0;
        bAlternateSetting=0;
      }
    } __attribute__((packed));
  }
}


