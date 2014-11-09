/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /*
   * Template class for USB HID devices
   */

  template<class TPhy,template <class> class... Features>
  struct UsbHidDevice : UsbDevice<TPhy>,
                        UsbControlEndpointFeature<UsbDevice<TPhy>>,
                        Features<UsbDevice<TPhy>>... {

    /*
     * Parameters for the HID device
     */

    struct Parameters : UsbDevice<TPhy>::Parameters,
                        UsbControlEndpointFeature<UsbDevice<TPhy>>::Parameters,
                        Features<UsbDevice<TPhy>>::Parameters... {
    };


    /*
     * Constructor
     */

    UsbHidDevice(Parameters& params)
      : UsbDevice<TPhy>(params),
        UsbControlEndpointFeature<UsbDevice<TPhy>>(*this,params),
        Features<UsbDevice<TPhy>>(*this,params)... {

      // link the HID interface/endpoint registration into the SDK structure

      USBD_RegisterClass(&this->_deviceHandle,USBD_HID_CLASS);
    }
  };
}
