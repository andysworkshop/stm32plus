/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Top-level base for all USB device features. Provides a storage container for the
   * USB device reference
   */

  template<class TUsbDevice>
  class UsbDeviceFeatureBase {

    protected:
      TUsbDevice& _device;

    protected:
      UsbDeviceFeatureBase(TUsbDevice& device);
  };


  /**
   * Constructor
   * @param device A reference to the USB device class
   */

  template<class TUsbDevice>
  inline UsbDeviceFeatureBase<TUsbDevice>::UsbDeviceFeatureBase(TUsbDevice& device)
    : _device(device) {
  }
}
