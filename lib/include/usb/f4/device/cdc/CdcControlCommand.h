/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * CDC control commands enumeration
     */

    enum class CdcControlCommand : uint8_t {

      SEND_ENCAPSULATED_COMMAND = 0x00, //!< CDC_SEND_ENCAPSULATED_COMMAND
      GET_ENCAPSULATED_RESPONSE = 0x01, //!< CDC_GET_ENCAPSULATED_RESPONSE
      SET_COMM_FEATURE          = 0x02, //!< CDC_SET_COMM_FEATURE
      GET_COMM_FEATURE          = 0x03, //!< CDC_GET_COMM_FEATURE
      CLEAR_COMM_FEATURE        = 0x04, //!< CDC_CLEAR_COMM_FEATURE
      SET_LINE_CODING           = 0x20, //!< CDC_SET_LINE_CODING
      GET_LINE_CODING           = 0x21, //!< CDC_GET_LINE_CODING
      SET_CONTROL_LINE_STATE    = 0x22, //!< CDC_SET_CONTROL_LINE_STATE
      SEND_BREAK                = 0x23, //!< CDC_SEND_BREAK

      NONE = 0xff
    };
  }
}
