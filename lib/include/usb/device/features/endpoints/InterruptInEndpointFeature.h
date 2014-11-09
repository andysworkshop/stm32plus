/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Intermediate base class for IN endpoints
     */

    template<uint8_t TEndpointNumber,class TDevice>
    class InterruptInEndpointFeature : public InEndpointFeatureBase<TDevice> {

      public:
        struct Parameters : InEndpointFeatureBase<TDevice>::Parameters {
        };

      protected:
        InterruptInEndpointFeature(TDevice& device,const Parameters& params);
    };


    /**
     * Constructor
     * @param device Reference to the device
     * @param params Reference to the parameters
     */

    template<uint8_t TEndpointNumber,class TDevice>
    inline InterruptInEndpointFeature<TEndpointNumber,TDevice>::InterruptInEndpointFeature(TDevice& device,const Parameters& params)
      : InEndpointFeatureBase<TDevice>(device,params,TEndpointNumber) {
    }
  }
}
