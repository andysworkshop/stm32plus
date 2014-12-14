/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * A non-exhaustive list of device class codes
     */

    enum class DeviceClass : uint8_t {
      AUDIO = 0x1,
      COMMUNICATIONS_CDC_CONTROL = 0x2,
      HID = 0x3,
      PHYSICAL = 0x5,
      IMAGE = 0x6,
      PRINTER = 0x7,
      MASS_STORAGE = 0x8,
      HUB = 0x9,
      CDC_DATA
    };
  }
}
