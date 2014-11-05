/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {


  /*
   * Event class for opening and endpoint
   */

  struct UsbDeviceSdkIsStalledEndpointEvent : UsbDeviceSdkEndpointEvent {

    bool isStalled;

    UsbDeviceSdkIsStalledEndpointEvent(uint8_t ep_addr)
      : UsbDeviceSdkEndpointEvent(EventType::IS_STALL_ENDPOINT,ep_addr),
        isStalled(false) {
    }
  };
}
