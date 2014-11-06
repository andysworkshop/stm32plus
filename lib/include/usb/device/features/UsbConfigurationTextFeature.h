/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Provide a configuration string. This is optional
   */

  template<class TUsbDevice>
  class UsbConfigurationTextFeature : public UsbStringFeatureBase<TUsbDevice> {

    public:
      struct Parameters {
        const char *device_configuration_text;
      };

    public:
      UsbConfigurationTextFeature(TUsbDevice& device,const Parameters& params);
  };


  /**
   * Constructor
   * @param device A reference to the USB device
   * @param str The configuration string
   */

  template<class TUsbDevice>
  inline UsbConfigurationTextFeature<TUsbDevice>::UsbConfigurationTextFeature(TUsbDevice& device,const Parameters& params)
    : UsbStringFeatureBase<TUsbDevice>(device,params.device_configuration_text,USBD_IDX_CONFIG_STR) {
  }
}
