/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {


  /*
   * Base class for all USB device events
   */

  struct UsbDeviceEventDescriptor {

    enum class EventType {
      INIT,
      DEINIT,
      START,
      STOP,
      OPEN_ENDPOINT,
      CLOSE_ENDPOINT,
      FLUSH_ENDPOINT,
      STALL_ENDPOINT,
      CLEAR_STALL_ENDPOINT,
      IS_STALL_ENDPOINT,
      SET_USB_ADDRESS,
      TRANSMIT,
      PREPARE_RECEIVE,
      GET_RX_DATA_SIZE,

      GET_DEVICE_DESCRIPTOR,
      GET_LANGUAGE_DESCRIPTOR,
      GET_STRING_DESCRIPTOR
    };

    EventType eventType;

    UsbDeviceEventDescriptor(EventType type)
      : eventType(type) {
    }
  };
}
