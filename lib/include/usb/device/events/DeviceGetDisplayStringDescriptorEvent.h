/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Get an optional string descriptor. The appropriate feature class will
     * process this event
     */

    struct DeviceGetDisplayStringDescriptorEvent : UsbEventDescriptor {

      /*
       * Inputs
       */

      USBD_SpeedTypeDef speed;
      uint8_t stringIndex;

      /*
       * Outputs
       */

      uint8_t *descriptor;
      uint16_t length;


      DeviceGetDisplayStringDescriptorEvent(USBD_SpeedTypeDef s,uint8_t stringIndex)
        : UsbEventDescriptor(EventType::DEVICE_GET_STRING_DESCRIPTOR),
          speed(s),
          stringIndex(stringIndex),
          descriptor(nullptr),
          length(0) {
      }
    };
  }
}
