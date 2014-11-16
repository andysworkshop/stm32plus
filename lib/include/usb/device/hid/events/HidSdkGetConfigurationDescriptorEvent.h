/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for HID config descriptor retrieval
     */

    struct HidSdkGetConfigurationDescriptorEvent : UsbEventDescriptor {

      uint8_t *descriptor;
      uint16_t length;

      HidSdkGetConfigurationDescriptorEvent(uint8_t *desc,uint16_t len)
        : UsbEventDescriptor(EventType::HID_GET_CONFIGURATION_DESCRIPTOR),
          descriptor(desc),
          length(len) {
      }
    };
  }
}
