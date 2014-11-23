/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Template class for USB HID devices
     * @tparam TPhy the PHY implementation
     * @tparam Features... The device feature classes
     */

    template<class TPhy,template <class> class... Features>
    class HidDevice : public Device<TPhy>,
                      public ControlEndpointFeature<Device<TPhy>>,
                      public Features<Device<TPhy>>... {


      public:

        /*
         * Parameters for the HID device
         */

        struct Parameters : Device<TPhy>::Parameters,
                            ControlEndpointFeature<Device<TPhy>>::Parameters,
                            Features<Device<TPhy>>::Parameters... {
        };

      protected:
        volatile uint8_t _hidProtocol;
        volatile uint8_t _hidIdleState;
        volatile uint8_t _hidAltSetting;
        volatile bool _busy;
        volatile bool _isReportAvailable;

      protected:
        void onEvent(UsbEventDescriptor& event);
        void onHidSetupEvent(DeviceClassSdkSetupEvent& event);
        void onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event);

      public:
        HidDevice();
        ~HidDevice();

        bool initialise(Parameters& params);

        bool isBusy() const;
        bool hidSendReport(const InEndpointFeatureBase<Device<TPhy>>& endpoint,const void *data,uint16_t len);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline HidDevice<TPhy,Features...>::HidDevice()
      : ControlEndpointFeature<Device<TPhy>>(static_cast<Device<TPhy>&>(*this)),
        Features<Device<TPhy>>(static_cast<Device<TPhy>&>(*this))... {

      _busy=false;
      _isReportAvailable=false;

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&HidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline HidDevice<TPhy,Features...>::~HidDevice() {

      // subscribe to USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&HidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool HidDevice<TPhy,Features...>::initialise(Parameters& params) {

      // initialise upwards

      if(!Device<TPhy>::initialise(params) ||
         !ControlEndpointFeature<Device<TPhy>>::initialise(params) ||
         !RecursiveBoolInitWithParams<HidDevice,Features<Device<TPhy>>...>::tinit(this,params))
        return false;

      // link UsbEventSource class into the SDK structure

      USBD_RegisterClass(&this->_deviceHandle,static_cast<UsbEventSource *>(this));
      return true;
    }


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline)) inline void HidDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

      // check for handled events

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_SETUP:
          onHidSetupEvent(static_cast<DeviceClassSdkSetupEvent&>(event));
          break;

        case UsbEventDescriptor::EventType::CLASS_INIT:
          _busy=false;
          break;

        case UsbEventDescriptor::EventType::CLASS_DATA_IN:
          _busy=false;
          break;

        case UsbEventDescriptor::EventType::CLASS_GET_DEVICE_QUALIFIER_DESCRIPTOR:
          onHidGetDeviceQualifierDescriptor(static_cast<DeviceClassSdkGetDeviceQualifierDescriptorEvent&>(event));
          break;

        default:
          break;
      }
    }


    /**
     * Handle a HID setup event
     * @param event the setup event
     */

    template<class TPhy,template <class> class... Features>
    inline void HidDevice<TPhy,Features...>::onHidSetupEvent(DeviceClassSdkSetupEvent& event) {

      // handle device class requests

      if((event.request.bmRequest & USB_REQ_TYPE_MASK)!=USB_REQ_TYPE_CLASS)
        return;

      // handle the common stuff

      switch(static_cast<HidClassRequestType>(event.request.bRequest)) {

        case HidClassRequestType::SET_PROTOCOL:
          _hidProtocol=event.request.wValue;
          break;

        case HidClassRequestType::GET_PROTOCOL:
          USBD_CtlSendData(&this->_deviceHandle,(uint8_t *)&_hidProtocol,1);
          break;

        case HidClassRequestType::SET_IDLE:
          _hidIdleState=event.request.wValue >> 8;
          break;

        case HidClassRequestType::GET_IDLE:
          USBD_CtlSendData(&this->_deviceHandle,(uint8_t *)&_hidIdleState,1);
          break;

        default:
          break;
      }
    }


    /**
     * Get the device qualifier descriptor
     * @param event The event class to receive the descriptor pointer
     */

    template<class TPhy,template <class> class... Features>
    inline void HidDevice<TPhy,Features...>::onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event) {

      event.descriptor=&this->_qualifierDescriptor;
      event.length=sizeof(this->_qualifierDescriptor);
    }


    /**
     * Send a HID 'report' to an IN endpoint
     * @param endpoint The endpoint to send to
     * @param data The data to send
     * @param len The size of the data
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool HidDevice<TPhy,Features...>::hidSendReport(const InEndpointFeatureBase<Device<TPhy>>& endpoint,const void *data,uint16_t len) {

      // must be configured

      if(this->_deviceHandle.dev_state!=USBD_STATE_CONFIGURED)
        return this->setError(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,this->E_UNCONFIGURED);

      // must be idle

      if(_busy)
        return this->setError(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,this->E_BUSY);

      // OK (XXX: check this for state mgmt if USBD_LL_Transmit fails)

      _busy=true;
      return endpoint.endpointTransmit(data,len);
    }


    /**
     * Check if we're busy (data IN to host in progress)
     */

    template<class TPhy,template <class> class... Features>
    inline bool HidDevice<TPhy,Features...>::isBusy() const {
      return _busy;
    }
  }
}
