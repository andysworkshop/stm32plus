/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace usb {


    /*
     * The event signature for USB device events
     */

    DECLARE_EVENT_SIGNATURE(Device,void(DeviceEventDescriptor&));


    /*
     * Base structure that holds just the event subscriber/publisher for USB devices
     */

    struct DeviceEventSource {
      DECLARE_EVENT_SOURCE(Device);
    };
  }
}
