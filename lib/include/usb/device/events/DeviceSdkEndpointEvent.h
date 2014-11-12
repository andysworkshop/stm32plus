/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for SDK events that just tell us something has happened to an endpoint
     */

    struct DeviceSdkEndpointEvent : UsbEventDescriptor {

      HalUsbStatus retval;
      uint8_t endpointAddress;

      DeviceSdkEndpointEvent(EventType eventType,uint8_t ep_addr)
        : UsbEventDescriptor(eventType),
          endpointAddress(ep_addr) {
      }
    };
  }
}
