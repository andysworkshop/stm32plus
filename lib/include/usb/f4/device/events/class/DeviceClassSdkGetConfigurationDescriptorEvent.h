/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for class config descriptor retrieval
     */

    struct DeviceClassSdkGetConfigurationDescriptorEvent : UsbEventDescriptor {

      enum class Type : uint8_t {
        HIGH_SPEED,
        FULL_SPEED,
        OTHER_SPEED,
        USER
      };

      Type type;
      uint8_t *descriptor;
      uint16_t length;

      DeviceClassSdkGetConfigurationDescriptorEvent(Type t)
        : UsbEventDescriptor(EventType::CLASS_GET_CONFIGURATION_DESCRIPTOR),
          type(t),
          descriptor(nullptr),
          length(0) {
      }
    };
  }
}
