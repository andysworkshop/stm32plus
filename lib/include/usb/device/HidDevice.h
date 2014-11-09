/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * Template class for USB HID devices
     */

    template<class TPhy,template <class> class... Features>
    struct HidDevice : Device<TPhy>,
                       ControlEndpointFeature<Device<TPhy>>,
                       Features<Device<TPhy>>... {

      /*
       * Parameters for the HID device
       */

      struct Parameters : Device<TPhy>::Parameters,
                          ControlEndpointFeature<Device<TPhy>>::Parameters,
                          Features<Device<TPhy>>::Parameters... {
      };


      /*
       * Constructor
       */

      HidDevice(Parameters& params)
        : Device<TPhy>(params),
          ControlEndpointFeature<Device<TPhy>>(*this,params),
          Features<Device<TPhy>>(*this,params)... {

        // link the HID interface/endpoint registration into the SDK structure

        USBD_RegisterClass(&this->_deviceHandle,USBD_HID_CLASS);
      }
    };
  }
}
