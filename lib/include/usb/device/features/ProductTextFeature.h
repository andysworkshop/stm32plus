/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Provide a product string. This is optional
     */

    template<class TDevice>
    class ProductTextFeature : public StringFeatureBase<TDevice> {

      public:
        struct Parameters {
          const char *device_product_text;
        };

      protected:
        ProductTextFeature(TDevice& device);

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param device A reference to the USB device
     */

    template<class TDevice>
    inline ProductTextFeature<TDevice>::ProductTextFeature(TDevice& device)
      : StringFeatureBase<TDevice>(device,USBD_IDX_PRODUCT_STR) {

      // register this string index with the device descriptor

      device.getDeviceDescriptor().iProduct=USBD_IDX_PRODUCT_STR;
    }


    /**
     * Initialise the class
     * @param params The parameters
     * @return true
     */

    template<class TDevice>
    inline bool ProductTextFeature<TDevice>::initialise(const Parameters& params) {
      return StringFeatureBase<TDevice>::initialise(params.device_product_text);
    }
  }
}
