/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * USB control endpoint. The special bi-directional endpoint that's always at position zero
   */

  template<class TUsbDevice>
  class UsbControlEndpointFeature : public UsbDeviceFeatureBase<TUsbDevice> {

    public:
      struct Parameters {
        uint16_t ctrlep_txFifoSize;     // default is 0x40

        Parameters() {
          ctrlep_txFifoSize=0x40;
        }
      };

    public:
      UsbControlEndpointFeature(TUsbDevice& device,const Parameters& params);
  };


  /**
   * Constructor
   * @param device A reference to the USB device
   * @param str The manufacturer string
   */

  template<class TUsbDevice>
  inline UsbControlEndpointFeature<TUsbDevice>::UsbControlEndpointFeature(TUsbDevice& device,const Parameters& params)
    : UsbDeviceFeatureBase<TUsbDevice>(device) {

    // increase endpoints counter

    device.incrementNumEndpoints();

    // set the tx fifo size

    HAL_PCD_SetTxFiFo(&device.getPcdHandle(),0,params.ctrlep_txFifoSize);
  }
}
