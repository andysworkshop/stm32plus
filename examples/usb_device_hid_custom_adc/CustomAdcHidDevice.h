/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// The HID report descriptor is defined in usb_device_hid_custom_adc.cpp

extern const uint8_t CustomAdcHidReportDescriptor[22];


namespace stm32plus {
  namespace usb {

    /**
     * Endpoint used by this device: a single IN endpoint to send data inbound to the host
     */

    template<class TDevice> using CustomAdcHidDeviceEndpoint=InterruptInEndpointFeature<1,TDevice>;

    /**
     * Derivation of HidDevice to handle a custom HID device that sends 3 ADC conversions to the
     * host every 1 second. ADC-1 channels 1,2,3 (PA1,PA2,PA3) are the channels used. Each conversion
     * takes 2-bytes so we are sending 6-byte reports.
     *
     * This HID device declares the following configuration structure:
     *
     *   1x Configuration descriptor
     *   1x Interface
     *   1x Inbound interrupt endpoint
     */

    template<class TPhy,template <class> class... Features>
    class CustomAdcHidDevice : public HidDevice<TPhy,CustomAdcHidDeviceEndpoint,Features...> {

      protected:

        typedef HidDevice<TPhy,CustomAdcHidDeviceEndpoint,Features...> HidDeviceBase;

        /**
         * Constants
         */

        enum {
          CUSTOM_ADC_HID_REPORT_SIZE = 6
        };


        /**
         * Declare the structure that gets sent back when the host asks for the whole
         * configuration descriptor
         */

        struct CustomAdcHidConfigurationDescriptor {
          ConfigurationDescriptor configuration;
          InterfaceDescriptor interface;
          HidClassDescriptor hid;
          EndpointDescriptor endpoint;
        } __attribute__((packed));

        CustomAdcHidConfigurationDescriptor _hidDescriptor;

      public:

        /**
         * Customisable parameters for this HID device
         */

        struct Parameters : HidDeviceBase::Parameters {

          uint8_t custom_hid_poll_interval;      // default is 100

          Parameters() {
            custom_hid_poll_interval=100;
          }
        };

      protected:
        void onEvent(UsbEventDescriptor& event);

        // event processing for the events that we'll need to take action upon

        void onHidInit();
        void onHidDeInit();
        void onHidSetup(DeviceClassSdkSetupEvent& event);
        void onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event);

      public:
        CustomAdcHidDevice();
        ~CustomAdcHidDevice();

        bool initialise(Parameters& params);

        bool sendAdcReport(uint16_t channel1,uint16_t channel2,uint16_t channel3);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline CustomAdcHidDevice<TPhy,Features...>::CustomAdcHidDevice() {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&CustomAdcHidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline CustomAdcHidDevice<TPhy,Features...>::~CustomAdcHidDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&CustomAdcHidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool CustomAdcHidDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!HidDeviceBase::initialise(params))
        return false;

      // set up the configuration descriptor (see constructor for defaults)

      _hidDescriptor.configuration.wTotalLength=sizeof(_hidDescriptor);
      _hidDescriptor.configuration.bNumInterfaces=1;
      _hidDescriptor.configuration.bmAttributes=0x80 | ConfigurationDescriptor::SELF_POWERED;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,CustomAdcHidDevice<TPhy,Features...>>::value)
        _hidDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor (see constructor for defaults)

      _hidDescriptor.interface.bInterfaceNumber=0;
      _hidDescriptor.interface.bNumEndpoints=1;
      _hidDescriptor.interface.bInterfaceClass=static_cast<uint8_t>(DeviceClass::HID);
      _hidDescriptor.interface.bInterfaceSubClass=static_cast<uint8_t>(HidSubClass::NONE);    // custom HID
      _hidDescriptor.interface.bInterfaceProtocol=static_cast<uint8_t>(HidProtocol::NONE);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,CustomAdcHidDevice<TPhy,Features...>>::value)
        _hidDescriptor.interface.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the hid class descriptor (see constructor for defaults)

      _hidDescriptor.hid.bNumDescriptors=1;
      _hidDescriptor.hid.wItemLength=sizeof(CustomAdcHidReportDescriptor);

      // set up the endpoint descriptor. we'll configure a max packet size of 64 though the
      // actual number of bytes per report will be 1+6=7

      _hidDescriptor.endpoint.bEndpointAddress=EndpointDescriptor::IN | 1;
      _hidDescriptor.endpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      _hidDescriptor.endpoint.wMaxPacketSize=USB_FS_MAX_PACKET_SIZE;
      _hidDescriptor.endpoint.bInterval=params.custom_hid_poll_interval;  // default is 1000ms

      // set up the qualifier descriptor (see constructor for defaults)

      this->_qualifierDescriptor.bMaxPacketSize0=0x40;
      this->_qualifierDescriptor.bNumConfigurations=1;

      // start the device

      if((status=USBD_Start(&this->_deviceHandle))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,this->E_START,status);

      // OK

      return true;
    }


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline)) inline void CustomAdcHidDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_INIT:
          onHidInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          onHidDeInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_GET_CONFIGURATION_DESCRIPTOR:
          onHidGetConfigurationDescriptor(static_cast<DeviceClassSdkGetConfigurationDescriptorEvent&>(event));
          break;

        case UsbEventDescriptor::EventType::CLASS_SETUP:
          onHidSetup(static_cast<DeviceClassSdkSetupEvent&>(event));
          break;

        default:
          break;
      }
    }


    /**
     * HID initialisation
     */

    template<class TPhy,template <class> class... Features>
    inline void CustomAdcHidDevice<TPhy,Features...>::onHidInit() {

      // open an interrupt IN endpoint

      USBD_LL_OpenEP(&this->_deviceHandle,
                     EndpointDescriptor::IN | 1,
                     EndpointDescriptor::INTERRUPT,
                     CUSTOM_ADC_HID_REPORT_SIZE);
    }


    /**
     * De-initialise the HID device
     */

    template<class TPhy,template <class> class... Features>
    inline void CustomAdcHidDevice<TPhy,Features...>::onHidDeInit() {

      // close the endpoint

      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::IN | 1);
    }


    /**
     * Get the configuration descriptor
     * @param event The event class to receive the descriptor and provide type of descriptor being requested
     */

    template<class TPhy,template <class> class... Features>
    inline void CustomAdcHidDevice<TPhy,Features...>::onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event) {

      if(event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::HIGH_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::FULL_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::OTHER_SPEED) {

        // set up the values in the event

        event.length=sizeof(_hidDescriptor);
        event.descriptor=reinterpret_cast<uint8_t *>(&_hidDescriptor);
      }
    }


    /**
     * Handle the HID setup requests
     * @param event the event containg value being requested
     */

    template<class TPhy,template <class> class... Features>
    inline void CustomAdcHidDevice<TPhy,Features...>::onHidSetup(DeviceClassSdkSetupEvent& event) {

      // check for fail

      if(event.status!=USBD_OK)
        return;

      // process the setup event handled here

      if((event.request.bmRequest & USB_REQ_TYPE_MASK)!=USB_REQ_TYPE_STANDARD)
        return;

      switch(event.request.bRequest) {

        case USB_REQ_GET_DESCRIPTOR:
          if(event.request.wValue >> 8 == HidClassDescriptor::HID_REPORT_DESCRIPTOR) {

            USBD_CtlSendData(&this->_deviceHandle,
                             const_cast<uint8_t *>(CustomAdcHidReportDescriptor),
                             Min<uint16_t>(sizeof(CustomAdcHidReportDescriptor),event.request.wLength));

          } else if(event.request.wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {

            USBD_CtlSendData(&this->_deviceHandle,
                             reinterpret_cast<uint8_t *>(&_hidDescriptor.hid),
                             Min<uint16_t>(sizeof(_hidDescriptor.hid),event.request.wLength));
          }

          break;

        case USB_REQ_GET_INTERFACE:
          USBD_CtlSendData(&this->_deviceHandle,(uint8_t *)&this->_hidAltSetting,1);
          break;

        case USB_REQ_SET_INTERFACE:
          this->_hidAltSetting=event.request.wValue;
          break;

        default:
          break;
      }
    }


    /**
     * Send a 3-byte HID report to the host
     * @param data The data to send
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool CustomAdcHidDevice<TPhy,Features...>::sendAdcReport(uint16_t channel1,uint16_t channel2,uint16_t channel3) {

      // data must remain in scope until IRQ indicates transmission complete

      static uint16_t data[3];

      data[0]=channel1;
      data[1]=channel2;
      data[2]=channel3;

      return HidDeviceBase::hidSendReport(
          static_cast<const CustomAdcHidDeviceEndpoint<Device<TPhy>>&>(*this),
          data,
          CUSTOM_ADC_HID_REPORT_SIZE);
    }
  }
}
