/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F0_42)

#include "config/usb/device/device.h"

namespace stm32plus {
  UsbInterruptHandler usbInterrupt;
}

#if defined(USE_USB_INTERRUPT)

extern "C" {

  /*
   * USB interrupt handler
   */

  void __attribute__ ((interrupt("IRQ"))) USB_IRQHandler(void) {
    stm32plus::usbInterrupt.UsbInterruptEventSender.raiseEvent();
    __DSB();
  }
}

#endif
#endif
