/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {

  /*
   * The event signature for USB device events
   */

  DECLARE_EVENT_SIGNATURE(UsbDevice,void(UsbDeviceEventDescriptor&));


  /*
   * Base structure that holds just the event subscriber/publisher for USB devices
   */

  struct UsbDeviceEventSource {
    DECLARE_EVENT_SOURCE(UsbDevice);
  };
}
