/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Sent by the device to the application to notify it that the state of the LEDs
     * on the HID keyboard has changed. Methods are provided to test the bits that
     * map to the individual LEDs
     */

    struct HidKeyboardLedStateEvent : UsbEventDescriptor {

      uint8_t status;

      HidKeyboardLedStateEvent(uint8_t s)
        : UsbEventDescriptor(EventType::HID_KEYBOARD_LED_STATE),
          status(s) {
      }


      /**
       * Check for num lock
       * @return true if set
       */

      bool isNumLock() const {
        return (status & 1)!=0;
      }


      /**
       * Check for caps lock
       * @return true if set
       */

      bool isCapsLock() const {
        return (status & 2)!=0;
      }


      /**
       * Check for scroll lock
       * @return true if set
       */

      bool isScrollLock() const {
        return (status & 4)!=0;
      }


      /**
       * Check for compose
       * @return true if set
       */

      bool isCompose() const {
        return (status & 8)!=0;
      }


      /**
       * Check for kana
       * @return true if set
       */

      bool isKana() const {
        return (status & 16)!=0;
      }
    };
  }
}
