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

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
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

        TConfigurationDescriptor  _configurationDescriptor;
        const uint8_t *_reportDescriptor;
        uint16_t _reportDescriptorLength;

      protected:
        void onEvent(UsbEventDescriptor& event);
        void onHidSetupEvent(DeviceClassSdkSetupEvent& event);
        void onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event);

      public:
        HidDevice(const uint8_t *reportDescriptor,uint16_t reportDescriptorLength);
        ~HidDevice();

        bool initialise(Parameters& params);

        bool isBusy() const;
        bool hidSendReport(const InEndpointFeatureBase<Device<TPhy>>& endpoint,const void *data,uint16_t len);
    };


    /**
     * Constructor
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline HidDevice<TPhy,TConfigurationDescriptor,Features...>::HidDevice(const uint8_t *reportDescriptor,uint16_t reportDescriptorLength)
      : ControlEndpointFeature<Device<TPhy>>(static_cast<Device<TPhy>&>(*this)),
        Features<Device<TPhy>>(static_cast<Device<TPhy>&>(*this))...,
        _reportDescriptor(reportDescriptor),
        _reportDescriptorLength(reportDescriptorLength) {

      _busy=false;
      _isReportAvailable=false;

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&HidDevice<TPhy,TConfigurationDescriptor,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline HidDevice<TPhy,TConfigurationDescriptor,Features...>::~HidDevice() {

      // subscribe to USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&HidDevice<TPhy,TConfigurationDescriptor,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline bool HidDevice<TPhy,TConfigurationDescriptor,Features...>::initialise(Parameters& params) {

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

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    __attribute__((noinline)) inline void HidDevice<TPhy,TConfigurationDescriptor,Features...>::onEvent(UsbEventDescriptor& event) {

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

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline void HidDevice<TPhy,TConfigurationDescriptor,Features...>::onHidSetupEvent(DeviceClassSdkSetupEvent& event) {

      // handle device class requests

      if((event.request.bmRequest & USB_REQ_TYPE_MASK)==USB_REQ_TYPE_CLASS) {

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
      else if((event.request.bmRequest & USB_REQ_TYPE_MASK)==USB_REQ_TYPE_STANDARD) {

        switch(event.request.bRequest) {

          case USB_REQ_GET_DESCRIPTOR:
            if(event.request.wValue >> 8 == HidClassDescriptor::HID_REPORT_DESCRIPTOR) {

              USBD_CtlSendData(&this->_deviceHandle,
                               const_cast<uint8_t *>(_reportDescriptor),
                               Min<uint16_t>(_reportDescriptorLength,event.request.wLength));

            } else if(event.request.wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {

              USBD_CtlSendData(&this->_deviceHandle,
                               reinterpret_cast<uint8_t *>(&_configurationDescriptor.hid),
                               Min<uint16_t>(sizeof(_configurationDescriptor.hid),event.request.wLength));
            }

            break;

          case USB_REQ_GET_INTERFACE:
            USBD_CtlSendData(&this->_deviceHandle,(uint8_t *)&_hidAltSetting,1);
            break;

          case USB_REQ_SET_INTERFACE:
            _hidAltSetting=event.request.wValue;
            break;

          default:
            break;
        }
      }
    }


    /**
     * Get the device qualifier descriptor
     * @param event The event class to receive the descriptor pointer
     */

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline void HidDevice<TPhy,TConfigurationDescriptor,Features...>::onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event) {

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

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline bool HidDevice<TPhy,TConfigurationDescriptor,Features...>::hidSendReport(const InEndpointFeatureBase<Device<TPhy>>& endpoint,const void *data,uint16_t len) {

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

    template<class TPhy,class TConfigurationDescriptor,template <class> class... Features>
    inline bool HidDevice<TPhy,TConfigurationDescriptor,Features...>::isBusy() const {
      return _busy;
    }
  }
}
