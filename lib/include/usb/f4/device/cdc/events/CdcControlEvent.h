/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Sent by the device code to the application to notify it that a control
     * event has happened.
     */

    struct CdcControlEvent : UsbEventDescriptor {

      CdcControlCommand opcode;
      const uint8_t *data;
      uint8_t size;

      CdcControlEvent(CdcControlCommand op,const uint8_t *d,uint8_t s)
        : UsbEventDescriptor(EventType::CDC_CONTROL),
          opcode(op),
          data(d),
          size(s) {
      }
    };
  }
}

