/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * CSW status codes
     */

    enum class MscBotCswStatus : uint8_t {

      CMD_PASSED = 0,   //!< CMD_PASSED
      CMD_FAILED = 1,   //!< CMD_FAILED
      PHASE_ERROR = 2   //!< PHASE_ERROR
    };
  }
}
