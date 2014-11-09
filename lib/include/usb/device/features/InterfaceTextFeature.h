/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Provide an interface text string. This is optional
     */

    template<class TUsbDevice>
    class InterfaceTextFeature : public StringFeatureBase<TUsbDevice> {

      public:
        struct Parameters {
          const char *device_interface_text;
        };

      protected:
        InterfaceTextFeature(TUsbDevice& device,const Parameters& params);
    };


    /**
     * Constructor
     * @param device A reference to the USB device
     * @param str The interface text
     */

    template<class TUsbDevice>
    inline InterfaceTextFeature<TUsbDevice>::InterfaceTextFeature(TUsbDevice& device,const Parameters& params)
      : StringFeatureBase<TUsbDevice>(device,params.device_interface_text,USBD_IDX_INTERFACE_STR) {
    }
  }
}
