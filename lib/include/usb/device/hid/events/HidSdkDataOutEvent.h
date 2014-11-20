/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for HID data out from the host
     */

    struct HidSdkDataOutEvent : UsbEventDescriptor {

      uint8_t endpointNumber;
      uint8_t *buffer;
      uint16_t length;
      USBD_StatusTypeDef status;

      HidSdkDataOutEvent(uint8_t epnum,uint8_t *buf,uint16_t len)
        : UsbEventDescriptor(EventType::HID_DATA_OUT),
          endpointNumber(epnum),
          buffer(buf),
          length(len),
          status(USBD_OK) {
      }
    };
  }
}
