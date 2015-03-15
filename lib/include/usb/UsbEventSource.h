/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace usb {


    /*
     * The event signature for USB device events
     */

    DECLARE_EVENT_SIGNATURE(Usb,void(UsbEventDescriptor&));
    DECLARE_EVENT_SIGNATURE(UsbError,void(UsbErrorEvent&));


    /*
     * Base structure that holds just the event subscriber/publisher for USB devices
     */

    struct UsbEventSource {
      DECLARE_EVENT_SOURCE(Usb);
      DECLARE_EVENT_SOURCE(UsbError);


      /**
       * Set an error in the error provider and raise an event
       */

      bool setError(int provider,uint32_t code,uint32_t cause=0) const {
        UsbErrorEventSender.raiseEvent(UsbErrorEvent(provider,code,cause));
        return false;
      }
    };
  }
}
