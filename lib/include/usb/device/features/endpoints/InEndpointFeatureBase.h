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

        /**
         * Error codes
         */

        enum {
          E_TRANSMIT_FAILED = 0x1
        };

      protected:
        uint8_t _endpointNumber;
        volatile bool _endpointIsTransmitting;

      protected:
        InEndpointFeatureBase(TDevice& device,uint8_t endpointNumber);
        ~InEndpointFeatureBase();

        void onEvent(UsbEventDescriptor& event);

      public:
        bool initialise(const Parameters& params);

        bool isTransmitting() const;
        bool transmit(const void *data,uint16_t size);
    };


    /**
     * Constructor
     * @param device Reference to the device
     */

    template<class TDevice>
    inline InEndpointFeatureBase<TDevice>::InEndpointFeatureBase(TDevice& device,uint8_t endpointNumber)
      : DeviceFeatureBase<TDevice>(device) {

      _endpointNumber=endpointNumber;
      _endpointIsTransmitting=false;

      // subscribe to USB events

      device.UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&InEndpointFeatureBase<TDevice>::onEvent)
        );

      // increase the endpoint counter

      device.incrementNumEndpoints();
    }


    /**
     * Destructor
     */

    template<class TDevice>
    inline InEndpointFeatureBase<TDevice>::~InEndpointFeatureBase() {

      // unsubscribe from USB events

      this->_device.UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&InEndpointFeatureBase<TDevice>::onEvent)
        );
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


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TDevice>
    __attribute__((noinline)) inline void InEndpointFeatureBase<TDevice>::onEvent(UsbEventDescriptor& event) {

      // check for handled events

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_INIT:
        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          _endpointIsTransmitting=false;
          break;

        case UsbEventDescriptor::EventType::CLASS_DATA_IN:
          if(static_cast<DeviceClassSdkDataInEvent&>(event).endpointNumber==_endpointNumber)
            _endpointIsTransmitting=false;
          break;

        default:      // get a warning if we don't do this
          break;
      }
    }


    /**
     * transmit some data to the host
     * @param data The data to send
     * @param size The number of bytes
     * @return true if it worked
     */

    template<class TDevice>
    inline bool InEndpointFeatureBase<TDevice>::transmit(const void *data,uint16_t size) {

      USBD_StatusTypeDef status;

      // must be configured

      if(this->_device.getDeviceHandle().dev_state!=USBD_STATE_CONFIGURED)
        return this->_device.setError(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,TDevice::E_UNCONFIGURED);

      // must not be transmitting

      if(_endpointIsTransmitting)
        return this->_device.setError(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,TDevice::E_BUSY);

      // this must be set first to avoid a race condition if the transmit succeeds really quickly

      _endpointIsTransmitting=true;

      if((status=USBD_LL_Transmit(
          &this->_device.getDeviceHandle(),
          _endpointNumber | EndpointDescriptor::IN,
          (uint8_t *)data,
          size))!=USBD_OK) {

        _endpointIsTransmitting=false;
        return this->_device.setError(ErrorProvider::ERROR_PROVIDER_USB_IN_ENDPOINT,E_TRANSMIT_FAILED,status);
      }

      // it's OK

      return true;
    }


    /**
     * Check if the endpoint is transmitting
     * @return true if it is transmitting
     */

    template<class TDevice>
    inline bool InEndpointFeatureBase<TDevice>::isTransmitting() const {
      return _endpointIsTransmitting;
    }
  }
}
