/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Sent when connected and language descriptor is being asked for. Subscriber
     * can modify descriptor if required.
     */

    struct DeviceGetLanguageDescriptorEvent : UsbEventDescriptor {

      USBD_SpeedTypeDef speed;
      LanguageDescriptor& descriptor;

      DeviceGetLanguageDescriptorEvent(USBD_SpeedTypeDef s,LanguageDescriptor& desc)
        : UsbEventDescriptor(EventType::DEVICE_GET_LANGUAGE_DESCRIPTOR),
          speed(s),
          descriptor(desc) {
      }
    };
  }
}
