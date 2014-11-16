/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace usb {

    /*
     * Base class for all USB events
     */

    struct UsbEventDescriptor {

      enum class EventType {
        DEVICE_INIT,
        DEVICE_DEINIT,
        DEVICE_START,
        DEVICE_STOP,
        DEVICE_OPEN_ENDPOINT,
        DEVICE_CLOSE_ENDPOINT,
        DEVICE_FLUSH_ENDPOINT,
        DEVICE_STALL_ENDPOINT,
        DEVICE_CLEAR_STALL_ENDPOINT,
        DEVICE_IS_STALL_ENDPOINT,
        DEVICE_SET_USB_ADDRESS,
        DEVICE_TRANSMIT,
        DEVICE_PREPARE_RECEIVE,
        DEVICE_GET_RX_DATA_SIZE,
        DEVICE_GET_DEVICE_DESCRIPTOR,
        DEVICE_GET_LANGUAGE_DESCRIPTOR,
        DEVICE_GET_STRING_DESCRIPTOR,

        DEVICE_IRQ_RESET,
        DEVICE_IRQ_SUSPEND,
        DEVICE_IRQ_RESUME,
        DEVICE_IRQ_SETUP_STAGE,
        DEVICE_IRQ_CONNECT,
        DEVICE_IRQ_DISCONNECT,
        DEVICE_IRQ_DATA_OUT_STAGE,
        DEVICE_IRQ_DATA_IN_STAGE,

        OTG_FS_INTERRUPT,

        HID_INIT,
        HID_DEINIT,
        HID_SETUP,
        HID_DATA_IN,
        HID_GET_CONFIGURATION_DESCRIPTOR,
        HID_GET_DEVICE_QUALIFIER_DESCRIPTOR,

        USB_ERROR
      };

      EventType eventType;

      UsbEventDescriptor(EventType type)
        : eventType(type) {
      }
    };
  }
}
