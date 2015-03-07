/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Subclass codes for HID devices
     */

    enum class HidSubClass : uint8_t {
      NONE = 0,     //!< NONE
      BOOT = 1,     //!< BOOT
    };
  }
}
