/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB configuration descriptor
     */

    struct ConfigurationDescriptor {

      enum {
        SELF_POWERED = 0x40,
        REMOTE_WAKEUP = 0x20
      };

      DescriptorHeader header;

      uint16_t wTotalLength;       // Total length in bytes of data returned
      uint8_t bNumInterfaces;      // Number of Interfaces
      uint8_t bConfigurationValue; // Value to use as an argument to select this configuration
      uint8_t iConfiguration;      // Index of String Descriptor describing this configuration
      uint8_t bmAttributes;        // D7 Reserved, set to 1. (USB 1.0 Bus Powered) D6 Self Powered. D5 Remote Wakeup. D4..0 Reserved, set to 0.
      uint8_t bMaxPower;           // Maximum Power Consumption in 2mA units

      ConfigurationDescriptor()
        : header(sizeof(ConfigurationDescriptor),USB_DESC_TYPE_CONFIGURATION) {

        // defaults for some

        bConfigurationValue=1;
        iConfiguration=0;
        bmAttributes=0x80 | SELF_POWERED | REMOTE_WAKEUP;
        bMaxPower=0x32;             // 100mA

        static_assert(sizeof(ConfigurationDescriptor)==9,"Compiler error: sizeof(ConfigurationDescriptor)!=9");
      }
    } __attribute__((packed));
  }
}


