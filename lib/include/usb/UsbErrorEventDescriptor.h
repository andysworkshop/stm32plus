/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Event descriptor for a USB error. We hold the provider, code and cause
     * variables because early reports using the errorProvider may be overwritten
     * as the error works its way up the stack.
     */

    struct UsbErrorEventDescriptor : UsbEventDescriptor {

      int provider;
      uint32_t code;
      uint32_t cause;

      /**
       * Constructor
       * @param provider The error provider code
       * @param code The error code (provider specific)
       * @param cause extended error information, if any
       */

      UsbErrorEventDescriptor(int provider_,uint32_t code_,uint32_t cause_=0)
        : UsbEventDescriptor(UsbEventDescriptor::EventType::USB_ERROR),
          provider(provider_),
          code(code_),
          cause(cause_) {
        errorProvider.set(provider,code,cause);
      }
    };
  }
}
