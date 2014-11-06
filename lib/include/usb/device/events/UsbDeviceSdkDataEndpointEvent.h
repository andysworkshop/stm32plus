/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {


  /*
   * Event class for the transmit/prepare-to-receive events
   */

  struct UsbDeviceSdkDataEndpointEvent : UsbDeviceSdkEndpointEvent {

    uint8_t *data;
    uint16_t size;

    UsbDeviceSdkDataEndpointEvent(EventType eventType,uint8_t ep_addr,uint8_t *buffer,uint16_t buffer_size)
      : UsbDeviceSdkEndpointEvent(eventType,ep_addr),
        data(buffer),
        size(buffer_size) {
    }
  };
}
