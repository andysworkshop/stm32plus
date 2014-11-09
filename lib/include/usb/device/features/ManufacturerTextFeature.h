/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Provide a manufacturer string. This is optional
     */

    template<class TDevice>
    class ManufacturerTextFeature : public StringFeatureBase<TDevice> {

      public:
        struct Parameters {
          const char *device_manufacturer_text;
        };

      public:
        ManufacturerTextFeature(TDevice& device,const Parameters& params);
    };


    /**
     * Constructor
     * @param device A reference to the USB device
     * @param str The manufacturer string
     */

    template<class TDevice>
    inline ManufacturerTextFeature<TDevice>::ManufacturerTextFeature(TDevice& device,const Parameters& params)
      : StringFeatureBase<TDevice>(device,params.device_manufacturer_text,USBD_IDX_MFC_STR) {
    }
  }
}
