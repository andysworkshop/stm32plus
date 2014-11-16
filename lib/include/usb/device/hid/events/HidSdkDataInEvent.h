/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for HID data in
     */

    struct HidSdkDataInEvent : UsbEventDescriptor {

      uint8_t _endpointNumber;
      USBD_StatusTypeDef status;

      HidSdkDataInEvent(uint8_t epnum)
        : UsbEventDescriptor(EventType::HID_DATA_IN),
          _endpointNumber(epnum),
          status(USBD_OK) {
      }
    };
  }
}
