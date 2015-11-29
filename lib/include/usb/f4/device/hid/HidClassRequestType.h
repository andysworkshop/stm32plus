/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /*
     * Class request types
     */

    enum class HidClassRequestType : uint8_t {
      SET_PROTOCOL = 0x0B,
      GET_PROTOCOL = 0x03,
      SET_IDLE     = 0x0A,
      GET_IDLE     = 0x02,
      SET_REPORT   = 0x09,
      GET_REPORT   = 0x01
    };
  }
}
