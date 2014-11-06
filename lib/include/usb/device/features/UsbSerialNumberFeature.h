/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Provide a serial number string. This is optional
   */

  template<class TUsbDevice>
  class UsbSerialNumberFeature : public UsbStringFeatureBase<TUsbDevice> {

    public:
      struct Parameters {
        const char *device_serial;
      };

    public:
      UsbSerialNumberFeature(TUsbDevice& device,const Parameters& params);
  };


  /**
   * Constructor
   * @param device A reference to the USB device
   * @param str The serial number string
   */

  template<class TUsbDevice>
  inline UsbSerialNumberFeature<TUsbDevice>::UsbSerialNumberFeature(TUsbDevice& device,const Parameters& params)
    : UsbStringFeatureBase<TUsbDevice>(device,params.device_serial,USBD_IDX_SERIAL_STR) {
  }
}
