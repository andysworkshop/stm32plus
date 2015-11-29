/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Sent when connected and device descriptor is being asked for. Subscriber
     * can modify descriptor if required.
     */

    struct DeviceGetDeviceDescriptorEvent : UsbEventDescriptor {

      USBD_SpeedTypeDef speed;
      DeviceDescriptor& descriptor;

      DeviceGetDeviceDescriptorEvent(USBD_SpeedTypeDef s,DeviceDescriptor& desc)
        : UsbEventDescriptor(EventType::DEVICE_GET_DEVICE_DESCRIPTOR),
          speed(s),
          descriptor(desc) {
      }
    };
  }
}
