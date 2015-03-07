/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * BOT Device class requests
     */

    enum class MscBotClassRequest : uint8_t {

      GET_MAX_LUN = 0xFE,   //!< get the maximum LUN number
      RESET = 0xFF          //!< reset the device
    };
  }
}
