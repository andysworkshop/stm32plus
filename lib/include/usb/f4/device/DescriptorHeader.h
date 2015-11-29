/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * USB Descriptor header. They all start with a byte length followed by
     * a descriptor type constant
     */

    struct DescriptorHeader {

      uint8_t bLength;
      uint8_t bDescriptorType;

      DescriptorHeader(uint8_t len,uint8_t type)
        : bLength(len),
          bDescriptorType(type) {
      }

    } __attribute__((packed));
  }
}
