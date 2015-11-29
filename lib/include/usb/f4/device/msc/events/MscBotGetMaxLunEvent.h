/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Sent to request the maximum logical unit number (LUN). The default is zero.
     */

    struct MscBotGetMaxLunEvent : UsbEventDescriptor {

      uint8_t maxLun;

      MscBotGetMaxLunEvent()
        : UsbEventDescriptor(EventType::MSC_BOT_GET_MAX_LUN),
          maxLun(0) {
      }
    };
  }
}

