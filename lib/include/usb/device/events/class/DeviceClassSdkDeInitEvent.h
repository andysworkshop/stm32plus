/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for device class deinitialisation
     */

    struct DeviceClassSdkDeInitEvent : UsbEventDescriptor {

      uint8_t _configurationIndex;
      USBD_StatusTypeDef status;

      DeviceClassSdkDeInitEvent(uint8_t cfgidx)
        : UsbEventDescriptor(EventType::CLASS_DEINIT),
          _configurationIndex(cfgidx),
          status(USBD_OK) {
      }
    };
  }
}
