/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * USB control endpoint. The special bi-directional endpoint that's always at position zero.
     * This endpoint feature is automatically added to your features so don't add it yourself.
     */

    template<class TDevice>
    class ControlEndpointFeature : public DeviceFeatureBase<TDevice> {

      public:
        struct Parameters {
          uint16_t ctrlep_txFifoSize;     // default is 0x40

          Parameters() {
            ctrlep_txFifoSize=0x40;
          }
        };

      protected:
        ControlEndpointFeature(TDevice& device);

      public:
        bool initialise(const Parameters& params);
    };


    /**
     * Constructor
     * @param the device reference
     */

    template<class TDevice>
    inline ControlEndpointFeature<TDevice>::ControlEndpointFeature(TDevice& device)
      : DeviceFeatureBase<TDevice>(device) {

      // increase endpoints counter

      device.incrementNumEndpoints();
    }


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true;
     */

    template<class TDevice>
    inline bool ControlEndpointFeature<TDevice>::initialise(const Parameters& params) {

      // set the tx fifo size

      HAL_PCD_SetTxFiFo(&this->_device.getPcdHandle(),0,params.ctrlep_txFifoSize);
      return true;
    }
  }
}
