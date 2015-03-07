/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for device class qualifier descriptor retrieval
     */

    struct DeviceClassSdkGetDeviceQualifierDescriptorEvent : UsbEventDescriptor {

      DeviceQualifierDescriptor *descriptor;
      uint16_t length;

      DeviceClassSdkGetDeviceQualifierDescriptorEvent()
        : UsbEventDescriptor(EventType::CLASS_GET_DEVICE_QUALIFIER_DESCRIPTOR),
          descriptor(nullptr),
          length(0) {
      }
    };
  }
}
