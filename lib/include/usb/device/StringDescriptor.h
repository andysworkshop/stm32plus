/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Device descriptor structure, 18 bytes
     */

    struct StringDescriptor {

      uint8_t bLength;
      uint8_t bDescriptorType;
    } __attribute__((packed));
  }
}
