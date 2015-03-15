/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Event to get the 'is write protected' state. There is no default, receivers
     * must implement this.
     */

    struct MscBotIsWriteProtectedEvent : UsbEventDescriptor {

      bool isWriteProtected;
      uint8_t lun;

      MscBotIsWriteProtectedEvent(uint8_t l)
        : UsbEventDescriptor(EventType::MSC_BOT_IS_WRITE_PROTECTED),
          lun(l) {
      }
    };
  }
}

