/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB HID class descriptor
     */

    struct HidClassDescriptor {

      /**
       * HID descriptor type enumeration
       */

      enum {
        HID_DESCRIPTOR_TYPE   = 0x21,
        HID_REPORT_DESCRIPTOR = 0x22
      };

      DescriptorHeader header;

      uint16_t bcdHID;            // HID version number
      uint8_t bCountryCode;       // Country code
      uint8_t bNumDescriptors;    // Number of descriptors
      uint8_t bDescriptorType;    // Descriptor type
      uint16_t wItemLength;       // Length of the report descriptor

      HidClassDescriptor()
        : header(sizeof(HidClassDescriptor),HID_DESCRIPTOR_TYPE) {

        static_assert(sizeof(HidClassDescriptor)==9,"Compiler error: sizeof(EndpointDescriptor)!=9");

        bcdHID=0x0111;    // default version 1.11
        bCountryCode=0;
        bDescriptorType=HID_REPORT_DESCRIPTOR;
      }
    } __attribute__((packed));
  }
}
