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
          E_START = 2,
          E_UNCONFIGURED = 3,
          E_BUSY = 4
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

        bool hidSendReport(InEndpointFeatureBase<Device<TPhy>>& endpoint,const void *data,uint16_t len);
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

      // initialise upwards

      if(!Device<TPhy>::initialise(params) ||
         !ControlEndpointFeature<Device<TPhy>>::initialise(params) ||
         !RecursiveBoolInitWithParams<HidDevice,Features<Device<TPhy>>...>::tinit(this,params))
        return false;

      return true;
    }


    /**
     * Send a HID 'report' to an IN endpoint
     * @param endpoint The endpoint to send to
     * @param data The data to send
     * @param len The size of the data
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool HidDevice<TPhy,Features...>::hidSendReport(InEndpointFeatureBase<Device<TPhy>>& endpoint,const void *data,uint16_t len) {

      USBD_HID_HandleTypeDef *hhid;

      hhid=reinterpret_cast<USBD_HID_HandleTypeDef *>(this->_deviceHandle.pClassData);

      // must be configured

      if(this->_deviceHandle.dev_state!=USBD_STATE_CONFIGURED)
        return this->setError(ErrorProvider::ERROR_PROVIDER_USB_HID_DEVICE,E_UNCONFIGURED);

      // must be idle

      if(hhid->state!=HID_IDLE)
        return this->setError(ErrorProvider::ERROR_PROVIDER_USB_HID_DEVICE,E_BUSY);

      // OK (XXX: check this for state mgmt if USBD_LL_Transmit fails)

      hhid->state=HID_BUSY;
      return endpoint.endpointTransmit(data,len);
    }
  }
}
