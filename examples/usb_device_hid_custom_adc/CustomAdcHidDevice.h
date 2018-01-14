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
     * Declare the structure that gets sent back when the host asks for the whole
     * configuration descriptor
     */

    struct CustomAdcHidConfigurationDescriptor {
      ConfigurationDescriptor configuration;
      InterfaceDescriptor interface;
      HidClassDescriptor hid;
      EndpointDescriptor endpoint;
    } __attribute__((packed));

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
    class CustomAdcHidDevice : public HidDevice<TPhy,
                                                CustomAdcHidConfigurationDescriptor,
                                                CustomAdcHidDeviceEndpoint,
                                                Features...> {

      protected:

        typedef HidDevice<TPhy,
                          CustomAdcHidConfigurationDescriptor,
                          CustomAdcHidDeviceEndpoint,
                          Features...> HidDeviceBase;

        /**
         * Constants
         */

        enum {
          CUSTOM_ADC_HID_REPORT_SIZE = 7    // 1-byte report ID
        };

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
    inline CustomAdcHidDevice<TPhy,Features...>::CustomAdcHidDevice()
      : HidDeviceBase(CustomAdcHidReportDescriptor,sizeof(CustomAdcHidReportDescriptor)) {

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

      this->_configurationDescriptor.configuration.wTotalLength=sizeof(this->_configurationDescriptor);
      this->_configurationDescriptor.configuration.bNumInterfaces=1;
      this->_configurationDescriptor.configuration.bmAttributes=0x80 | ConfigurationDescriptor::SELF_POWERED;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,CustomAdcHidDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor (see constructor for defaults)

      this->_configurationDescriptor.interface.bInterfaceNumber=0;
      this->_configurationDescriptor.interface.bNumEndpoints=1;
      this->_configurationDescriptor.interface.bInterfaceClass=static_cast<uint8_t>(DeviceClass::HID);
      this->_configurationDescriptor.interface.bInterfaceSubClass=static_cast<uint8_t>(HidSubClass::NONE);    // custom HID
      this->_configurationDescriptor.interface.bInterfaceProtocol=static_cast<uint8_t>(HidProtocol::NONE);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,CustomAdcHidDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.interface.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the hid class descriptor (see constructor for defaults)

      this->_configurationDescriptor.hid.bNumDescriptors=1;
      this->_configurationDescriptor.hid.wItemLength=sizeof(CustomAdcHidReportDescriptor);

      // set up the endpoint descriptor. we'll configure a max packet size of 64 though the
      // actual number of bytes per report will be 1+6=7

      this->_configurationDescriptor.endpoint.bEndpointAddress=EndpointDescriptor::IN | 1;
      this->_configurationDescriptor.endpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      this->_configurationDescriptor.endpoint.wMaxPacketSize=USB_FS_MAX_PACKET_SIZE;
      this->_configurationDescriptor.endpoint.bInterval=params.custom_hid_poll_interval;  // default is 1000ms

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
    __attribute__((noinline)) void CustomAdcHidDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

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

        event.length=sizeof(this->_configurationDescriptor);
        event.descriptor=reinterpret_cast<uint8_t *>(&this->_configurationDescriptor);
      }
    }


    /**
     * Send a 3-byte HID report to the host
     * @param data The data to send
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool CustomAdcHidDevice<TPhy,Features...>::sendAdcReport(uint16_t channel1,uint16_t channel2,uint16_t channel3) {

      CustomAdcHidDeviceEndpoint<Device<TPhy>>& endpoint=static_cast<CustomAdcHidDeviceEndpoint<Device<TPhy>>&>(*this);

      // data must remain in scope until IRQ indicates transmission complete

      static uint8_t report[7];
      uint16_t *ptr;

      // serialized report has 1-byte id at the front, followed by actual data;

      ptr=reinterpret_cast<uint16_t *>(&report[1]);

      report[0]=1;
      *ptr++=channel1;
      *ptr++=channel2;
      *ptr=channel3;

      // wait for previous send to complete by IRQ notification

      while(endpoint.isTransmitting());

      // send the report

      return endpoint.transmit(report,CUSTOM_ADC_HID_REPORT_SIZE);
    }
  }
}
