/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Class to handle interrupt IN endpoints
     */

    template<uint8_t TEndpointNumber,class TDevice>
    class InterruptInEndpointFeature : public InEndpointFeatureBase<TDevice> {

      public:
        struct Parameters : InEndpointFeatureBase<TDevice>::Parameters {
        };

      protected:
        InterruptInEndpointFeature(TDevice& device);

      public:
        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     * @param device Reference to the device
     */

    template<uint8_t TEndpointNumber,class TDevice>
    inline InterruptInEndpointFeature<TEndpointNumber,TDevice>::InterruptInEndpointFeature(TDevice& device)
      : InEndpointFeatureBase<TDevice>(device,TEndpointNumber) {
    }


    /**
     * Initialise the class
     * @param params the parameters
     */

    template<uint8_t TEndpointNumber,class TDevice>
    inline bool InterruptInEndpointFeature<TEndpointNumber,TDevice>::initialise(Parameters& params) {
      return InEndpointFeatureBase<TDevice>::initialise(params);
    }
  }
}
