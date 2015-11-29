/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * States for the BOT
     */

    enum class MscBotState : uint8_t  {
      IDLE,           //!< IDLE
      DATA_OUT,       //!< DATA_OUT
      DATA_IN,        //!< DATA_IN
      LAST_DATA_IN,   //!< LAST_DATA_IN
      SEND_DATA,      //!< SEND_DATA
      NO_DATA         //!< NO_DATA
    };
  }
}
