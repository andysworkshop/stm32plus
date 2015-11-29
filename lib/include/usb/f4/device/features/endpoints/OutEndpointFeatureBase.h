/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Intermediate base class for OUT endpoints
     */

    template<class TDevice>
    class OutEndpointFeatureBase : public DeviceFeatureBase<TDevice> {

      public:
        struct Parameters {
        };

      protected:
        uint8_t _endpointNumber;

      protected:
        OutEndpointFeatureBase(TDevice& device,uint8_t endpointNumber);

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param device Reference to the device
     */

    template<class TDevice>
    inline OutEndpointFeatureBase<TDevice>::OutEndpointFeatureBase(TDevice& device,uint8_t endpointNumber)
      : DeviceFeatureBase<TDevice>(device) {

      _endpointNumber=endpointNumber;

      // increase the endpoint counter

      device.incrementNumEndpoints();
    }


    /**
     * Initialise the class
     * @param params the parameters
     * @return true
     */

    template<class TDevice>
    inline bool OutEndpointFeatureBase<TDevice>::initialise(const Parameters& /* params */) {
      return true;
    }
  }
}
