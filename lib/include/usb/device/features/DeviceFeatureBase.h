/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Top-level base for all USB device features. Provides a storage container for the
     * USB device reference
     */

    template<class TUsbDevice>
    class DeviceFeatureBase {

      protected:
        TUsbDevice& _device;

      protected:
        DeviceFeatureBase(TUsbDevice& device);
    };


    /**
     * Constructor
     * @param device A reference to the USB device class
     */

    template<class TUsbDevice>
    inline DeviceFeatureBase<TUsbDevice>::DeviceFeatureBase(TUsbDevice& device)
      : _device(device) {
    }
  }
}
