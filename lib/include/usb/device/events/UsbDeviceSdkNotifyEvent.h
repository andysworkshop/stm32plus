/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {


  /*
   * Event class for SDK events that just tell us something has happened
   */

  struct UsbDeviceSdkNotifyEvent : UsbDeviceEventDescriptor {

    USBD_StatusTypeDef retval;

    UsbDeviceSdkNotifyEvent(EventType eventType)
      : UsbDeviceEventDescriptor(eventType),
        retval(USBD_OK) {
    }
  };
}
