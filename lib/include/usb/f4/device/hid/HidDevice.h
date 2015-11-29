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
     * @tparam TConfigurationDescriptor A structure that holds the complete config descriptor
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
        volatile bool _isReportAvailable;

        TConfigurationDescriptor  _configurationDescriptor;
        const uint8_t *_reportDescriptor;
        uint16_t _reportDescriptorLength;

      protected:
        void onEvent(UsbEventDescriptor& event);
        void onHidSetupEvent(DeviceClassSdkSetupEvent& event);

      public:
        HidDevice(const uint8_t *reportDescriptor,uint16_t reportDescriptorLength);
        ~HidDevice();

        bool initialise(Parameters& params);
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

      // unsubscribe from USB events

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

      // handle the setup event

      if(event.eventType==UsbEventDescriptor::EventType::CLASS_SETUP)
        onHidSetupEvent(static_cast<DeviceClassSdkSetupEvent&>(event));
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
                               std::min<uint16_t>(_reportDescriptorLength,event.request.wLength));

            } else if(event.request.wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {

              USBD_CtlSendData(&this->_deviceHandle,
                               reinterpret_cast<uint8_t *>(&_configurationDescriptor.hid),
                               std::min<uint16_t>(sizeof(_configurationDescriptor.hid),event.request.wLength));
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
  }
}
