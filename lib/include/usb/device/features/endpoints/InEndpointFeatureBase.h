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
        InEndpointFeatureBase(TDevice& device,const Parameters& params,uint8_t endpointNumber);
    };


    /**
     * Constructor
     * @param device Reference to the device
     * @param params Reference to the parameters
     */

    template<class TDevice>
    inline InEndpointFeatureBase<TDevice>::InEndpointFeatureBase(TDevice& device,const Parameters& params,uint8_t endpointNumber)
      : DeviceFeatureBase<TDevice>(device) {

      // set the FIFO size

      HAL_PCD_SetTxFiFo(&device.getPcdHandle(),endpointNumber,params.ep_txFifoSize);

      // increase the endpoint counter

      device.incrementNumEndpoints();
    }
  }
}
