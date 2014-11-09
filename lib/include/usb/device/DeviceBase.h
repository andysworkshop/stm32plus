/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Base class for USB devices. The only reason this class exists in the hierarchy is so
     * that the UsbDeviceEventSource is available to device features through a reference to
     * the UsbDevice class
     */

    template<class TPhy>
    struct DeviceBase : Core<TPhy>,
                        DeviceEventSource {

      struct Parameters : Core<TPhy>::Parameters {
      };

      DeviceBase(Parameters& params)
        : Core<TPhy>(params) {
      }
    };
  }
}
