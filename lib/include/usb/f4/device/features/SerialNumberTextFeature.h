/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Provide a serial number string. This is optional
     */

    template<class TDevice>
    class SerialNumberTextFeature : public StringFeatureBase<TDevice> {

      public:
        struct Parameters {
          const char *device_serial_text;
        };

      protected:
        SerialNumberTextFeature(TDevice& device);

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param device A reference to the USB device
     * @param str The serial number string
     */

    template<class TDevice>
    inline SerialNumberTextFeature<TDevice>::SerialNumberTextFeature(TDevice& device)
      : StringFeatureBase<TDevice>(device,USBD_IDX_SERIAL_STR) {

      // register this string index with the device descriptor

      device.getDeviceDescriptor().iSerialNumber=USBD_IDX_SERIAL_STR;
    }


    /**
     * Initialise the class
     * @param params The parameters
     * @return true
     */

    template<class TDevice>
    inline bool SerialNumberTextFeature<TDevice>::initialise(const Parameters& params) {
      return StringFeatureBase<TDevice>::initialise(params.device_serial_text);
    }
  }
}
