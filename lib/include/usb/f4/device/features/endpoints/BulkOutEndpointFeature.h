/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * Class to handle bulk OUT endpoints
     */

    template<uint8_t TEndpointNumber,class TDevice>
    class BulkOutEndpointFeature : public OutEndpointFeatureBase<TDevice> {

      public:
        struct Parameters : OutEndpointFeatureBase<TDevice>::Parameters {
        };

      protected:
        BulkOutEndpointFeature(TDevice& device);

      public:
        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     * @param device Reference to the device
     */

    template<uint8_t TEndpointNumber,class TDevice>
    inline BulkOutEndpointFeature<TEndpointNumber,TDevice>::BulkOutEndpointFeature(TDevice& device)
      : OutEndpointFeatureBase<TDevice>(device,TEndpointNumber) {
    }


    /**
     * Initialise the class
     * @param params the parameters
     */

    template<uint8_t TEndpointNumber,class TDevice>
    inline bool BulkOutEndpointFeature<TEndpointNumber,TDevice>::initialise(Parameters& params) {
      return OutEndpointFeatureBase<TDevice>::initialise(params);
    }
  }
}
