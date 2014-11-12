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

    template<class TDevice>
    class InEndpointFeatureBase : public DeviceFeatureBase<TDevice> {

      public:
        struct Parameters {
          uint16_t ep_txFifoSize;     // TX FIFO size, default is 0x80

          Parameters() {
            ep_txFifoSize=0x80;
          }
        };

      protected:
        uint8_t _endpointNumber;

      protected:
        InEndpointFeatureBase(TDevice& device,uint8_t endpointNumber);

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param device Reference to the device
     */

    template<class TDevice>
    inline InEndpointFeatureBase<TDevice>::InEndpointFeatureBase(TDevice& device,uint8_t endpointNumber)
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
    inline bool InEndpointFeatureBase<TDevice>::initialise(const Parameters& params) {

      // set the FIFO size

      HAL_PCD_SetTxFiFo(&this->_device.getPcdHandle(),_endpointNumber,params.ep_txFifoSize);
      return true;
    }
  }
}
