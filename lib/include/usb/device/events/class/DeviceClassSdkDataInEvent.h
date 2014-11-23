/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for device class data in
     */

    struct DeviceClassSdkDataInEvent : UsbEventDescriptor {

      uint8_t endpointNumber;
      USBD_StatusTypeDef status;

      DeviceClassSdkDataInEvent(uint8_t epnum)
        : UsbEventDescriptor(EventType::CLASS_DATA_IN),
          endpointNumber(epnum),
          status(USBD_OK) {
      }
    };
  }
}
