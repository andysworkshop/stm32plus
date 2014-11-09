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

    template<class TDevice>
    class DeviceFeatureBase {

      protected:
        TDevice& _device;

      protected:
        DeviceFeatureBase(TDevice& device);
    };


    /**
     * Constructor
     * @param device A reference to the USB device class
     */

    template<class TDevice>
    inline DeviceFeatureBase<TDevice>::DeviceFeatureBase(TDevice& device)
      : _device(device) {
    }
  }
}
