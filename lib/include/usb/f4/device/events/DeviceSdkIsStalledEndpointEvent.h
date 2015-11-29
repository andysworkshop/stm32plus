/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for checking if an endpoint is stalled. Default is false. Change
     * isStalled to true to indicate stalled.
     */

    struct DeviceSdkIsStalledEndpointEvent : DeviceSdkEndpointEvent {

      bool isStalled;

      DeviceSdkIsStalledEndpointEvent(uint8_t ep_addr)
        : DeviceSdkEndpointEvent(EventType::DEVICE_IS_STALL_ENDPOINT,ep_addr),
          isStalled(false) {
      }
    };
  }
}
