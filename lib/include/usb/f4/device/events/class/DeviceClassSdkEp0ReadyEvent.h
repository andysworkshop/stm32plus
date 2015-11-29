/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for device class control endpoint ready
     */

    struct DeviceClassSdkEp0ReadyEvent : UsbEventDescriptor {

      USBD_StatusTypeDef status;

      DeviceClassSdkEp0ReadyEvent()
        : UsbEventDescriptor(EventType::CLASS_EP0_READY),
          status(USBD_OK) {
      }
    };
  }
}
