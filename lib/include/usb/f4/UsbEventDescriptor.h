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

        CLASS_INIT,
        CLASS_DEINIT,
        CLASS_SETUP,
        CLASS_DATA_IN,
        CLASS_DATA_OUT,
        CLASS_EP0_READY,
        CLASS_SOF,
        CLASS_EP0_TX_SENT,
        CLASS_GET_CONFIGURATION_DESCRIPTOR,
        CLASS_GET_DEVICE_QUALIFIER_DESCRIPTOR,

        HID_KEYBOARD_LED_STATE,

        CDC_DATA_RECEIVED,
        CDC_CONTROL,

        MSC_BOT_GET_MAX_LUN,
        MSC_BOT_RESET,
        MSC_BOT_IS_READY,
        MSC_BOT_IS_WRITE_PROTECTED,
        MSC_BOT_READ,
        MSC_BOT_WRITE,
        MSC_BOT_GET_ENQUIRY_PAGE,
        MSC_BOT_GET_CAPACITY,

        USB_ERROR
      };

      EventType eventType;

      UsbEventDescriptor(EventType type)
        : eventType(type) {
      }
    };
  }
}
