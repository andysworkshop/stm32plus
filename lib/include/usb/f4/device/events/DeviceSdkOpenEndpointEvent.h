/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for opening and endpoint
     */

    struct DeviceSdkOpenEndpointEvent : DeviceSdkEndpointEvent {

      uint16_t maximumPacketSize;
      EndpointType endpointType;

      DeviceSdkOpenEndpointEvent(uint8_t ep_addr,EndpointType ep_type,uint16_t ep_mps)
        : DeviceSdkEndpointEvent(EventType::DEVICE_OPEN_ENDPOINT,ep_addr),
          maximumPacketSize(ep_mps),
          endpointType(ep_type) {
      }
    };
  }
}
