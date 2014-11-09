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

    template<class TUsbDevice>
    class ControlEndpointFeature : public DeviceFeatureBase<TUsbDevice> {

      public:
        struct Parameters {
          uint16_t ctrlep_txFifoSize;     // default is 0x40

          Parameters() {
            ctrlep_txFifoSize=0x40;
          }
        };

      protected:
        ControlEndpointFeature(TUsbDevice& device,const Parameters& params);
    };


    /**
     * Constructor
     * @param device A reference to the USB device
     * @param str The manufacturer string
     */

    template<class TUsbDevice>
    inline ControlEndpointFeature<TUsbDevice>::ControlEndpointFeature(TUsbDevice& device,const Parameters& params)
      : DeviceFeatureBase<TUsbDevice>(device) {

      // increase endpoints counter

      device.incrementNumEndpoints();

      // set the tx fifo size

      HAL_PCD_SetTxFiFo(&device.getPcdHandle(),0,params.ctrlep_txFifoSize);
    }
  }
}
