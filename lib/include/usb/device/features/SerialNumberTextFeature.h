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
        SerialNumberTextFeature(TDevice& device,const Parameters& params);
    };


    /**
     * Constructor
     * @param device A reference to the USB device
     * @param str The serial number string
     */

    template<class TDevice>
    inline SerialNumberTextFeature<TDevice>::SerialNumberTextFeature(TDevice& device,const Parameters& params)
      : StringFeatureBase<TDevice>(device,params.device_serial_text,USBD_IDX_SERIAL_STR) {
    }
  }
}
