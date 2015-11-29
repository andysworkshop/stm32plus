/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/usb/device/device.h"

using namespace stm32plus::usb;


#if defined(USE_OTG_FS_INTERRUPT)


// static initialisers

InternalFsPhyInterruptFeature::FPTR InternalFsPhyInterruptFeature::_forceLinkage=nullptr;
UsbEventSource *InternalFsPhyInterruptFeature::_usbEventSource=nullptr;


extern "C" {

  void __attribute__ ((interrupt("IRQ"))) OTG_FS_IRQHandler(void) {

    InternalFsPhyInterruptFeature::_usbEventSource->UsbEventSender.raiseEvent(OtgFsInterruptEvent());

    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif
#endif
