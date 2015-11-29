/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Event class for an SDK event asking for the last transferred packet size. The
     * default is zero so lastTransferred should be changed to the actual value
     */

    struct DeviceSdkGetLastTransferredSizeEndpointEvent : UsbEventDescriptor {

      uint8_t endpointAddress;
      uint32_t lastTransferred;

      DeviceSdkGetLastTransferredSizeEndpointEvent(uint8_t ep_addr)
        : UsbEventDescriptor(EventType::DEVICE_GET_RX_DATA_SIZE),
          endpointAddress(ep_addr),
          lastTransferred(0) {
      }
    };
  }
}
