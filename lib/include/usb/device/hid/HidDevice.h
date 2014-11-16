/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Template class for USB HID devices
     * @tparam TPhy the PHY implementation
     * @tparam Features... The device feature classes
     */

    template<class TPhy,template <class> class... Features>
    class HidDevice : public Device<TPhy>,
                      public ControlEndpointFeature<Device<TPhy>>,
                      public Features<Device<TPhy>>... {


      public:

        /**
         * Error codes
         */

        enum {
          E_REGISTER_CLASS = 1,
          E_START = 2
        };

        /*
         * Parameters for the HID device
         */

        struct Parameters : Device<TPhy>::Parameters,
                            ControlEndpointFeature<Device<TPhy>>::Parameters,
                            Features<Device<TPhy>>::Parameters... {
        };

      public:
        HidDevice();

        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     * @param params paramters class
     */

    template<class TPhy,template <class> class... Features>
    inline HidDevice<TPhy,Features...>::HidDevice()
      : ControlEndpointFeature<Device<TPhy>>(static_cast<Device<TPhy>&>(*this)),
        Features<Device<TPhy>>(static_cast<Device<TPhy>&>(*this))... {
    }


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool HidDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!Device<TPhy>::initialise(params) ||
         !ControlEndpointFeature<Device<TPhy>>::initialise(params) ||
         !RecursiveBoolInitWithParams<HidDevice,Features<Device<TPhy>>...>::tinit(this,params))
        return false;

      // link the HID interface/endpoint registration into the SDK structure

      if((status=USBD_RegisterClass(&this->_deviceHandle,USBD_HID_CLASS))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_HID_DEVICE,E_REGISTER_CLASS,status);

      // start the device

      if((status=USBD_Start(&this->_deviceHandle))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_HID_DEVICE,E_START,status);

      return true;
    }
  }
}
