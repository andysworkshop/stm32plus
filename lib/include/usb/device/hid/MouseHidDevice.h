/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Endpoint used by this device
     */

    template<class TDevice> using MouseHidDeviceEndpoint1=InterruptInEndpointFeature<1,TDevice>;

    /**
     * Derivation of HidDevice to handle a HID mouse. This device type declares the following:
     *   1x Configuration descriptor
     *   1x Interface
     *   1x Inbound interrupt endpoint
     *
     * The 3-byte HID report is for a simple 3-button mouse using the BOOT protocol. The format
     * of the 3-bytes is:
     *
     *  0: B7..3=unused. B2=left button. B1=middle button. B0=right button.
     *  1: X-axis relative movement as signed integer.
     *  2: Y-axis relative movement as signed integer.
     */

    template<class TPhy,template <class> class... Features>
    class MouseHidDevice : public HidDevice<TPhy,MouseHidDeviceEndpoint1,Features...> {

      protected:

        typedef HidDevice<TPhy,MouseHidDeviceEndpoint1,Features...> HidDeviceBase;

        /**
         * Constants
         */

        enum {
          MOUSE_HID_REPORT_SIZE = 3
        };


        /**
         * Declare the structure that gets sent back when the host asks for the whole
         * configuration descriptor
         */

        struct MouseConfigurationDescriptor {
          ConfigurationDescriptor configuration;
          InterfaceDescriptor interface;
          HidClassDescriptor hid;
          EndpointDescriptor endpoint;
        } __attribute__((packed));

        MouseConfigurationDescriptor _mouseDescriptor;
        DeviceQualifierDescriptor _qualifierDescriptor;

      public:

        /**
         * Customisable parameters for this HID device
         */

        struct Parameters : HidDeviceBase::Parameters {

          uint8_t hid_mouse_poll_interval;      // default is 10

          Parameters() {
            hid_mouse_poll_interval = 10;
          }
        };

      protected:
        void onEvent(UsbEventDescriptor& event);

        void onHidInit();
        void onHidDeInit();
        void onHidSetup(DeviceClassSdkSetupEvent& event);
        void onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event);
        void onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event);
        void onHidDataIn();

      public:
        MouseHidDevice();
        ~MouseHidDevice();

        bool initialise(Parameters& params);

        bool mouseSendReport(uint8_t buttons,int8_t x,int8_t y);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline MouseHidDevice<TPhy,Features...>::MouseHidDevice() {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&MouseHidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline MouseHidDevice<TPhy,Features...>::~MouseHidDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&MouseHidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool MouseHidDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!HidDeviceBase::initialise(params))
        return false;

      // set up the configuration descriptor (see constructor for defaults)

      _mouseDescriptor.configuration.wTotalLength=sizeof(_mouseDescriptor);
      _mouseDescriptor.configuration.bNumInterfaces=1;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,MouseHidDevice<TPhy,Features...>>::value)
        _mouseDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor (see constructor for defaults)

      _mouseDescriptor.interface.bInterfaceNumber=0;
      _mouseDescriptor.interface.bNumEndpoints=1;
      _mouseDescriptor.interface.bInterfaceClass=static_cast<uint8_t>(DeviceClass::HID);
      _mouseDescriptor.interface.bInterfaceSubClass=static_cast<uint8_t>(HidSubClass::BOOT);
      _mouseDescriptor.interface.bInterfaceProtocol=static_cast<uint8_t>(HidProtocol::MOUSE);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,MouseHidDevice<TPhy,Features...>>::value)
        _mouseDescriptor.interface.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the hid class descriptor (see constructor for defaults)

      _mouseDescriptor.hid.bNumDescriptors=1;
      _mouseDescriptor.hid.wItemLength=sizeof(MouseReportDescriptor);

      // set up the endpoint descriptor

      _mouseDescriptor.endpoint.bEndpointAddress=EndpointDescriptor::IN | 1;
      _mouseDescriptor.endpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      _mouseDescriptor.endpoint.wMaxPacketSize=MOUSE_HID_REPORT_SIZE;       // mouse reports are 3 bytes
      _mouseDescriptor.endpoint.bInterval=params.hid_mouse_poll_interval;   // default is 10 frames

      // set up the qualifier descriptor (see constructor for defaults)

      _qualifierDescriptor.bDeviceClass=0;
      _qualifierDescriptor.bDeviceSubClass=0;
      _qualifierDescriptor.bDeviceProtocol=0;
      _qualifierDescriptor.bMaxPacketSize0=0x40;
      _qualifierDescriptor.bNumConfigurations=1;

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
    __attribute__((noinline)) inline void MouseHidDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_INIT:
          onHidInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          onHidDeInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DATA_IN:
          onHidDataIn();
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
    inline void MouseHidDevice<TPhy,Features...>::onHidInit() {

      USBD_LL_OpenEP(&this->_deviceHandle,EndpointDescriptor::IN | 1,EndpointDescriptor::INTERRUPT,MOUSE_HID_REPORT_SIZE);

      // device is not busy

      this->_busy=false;
    }


    /**
     * De-initialise the HID device
     */

    template<class TPhy,template <class> class... Features>
    inline void MouseHidDevice<TPhy,Features...>::onHidDeInit() {

      // close the endpoint

      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::IN | 1);
    }


    /**
     * Get the configuration descriptor
     * @param event The event class to receive the descriptor and provide type of descriptor being requested
     */

    template<class TPhy,template <class> class... Features>
    inline void MouseHidDevice<TPhy,Features...>::onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event) {

      if(event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::HIGH_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::FULL_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::OTHER_SPEED) {

        // set up the values in the event

        event.length=sizeof(_mouseDescriptor);
        event.descriptor=reinterpret_cast<uint8_t *>(&_mouseDescriptor);
      }
    }


    /**
     * Get the device qualifier descriptor
     * @param event The event class to receive the descriptor pointer
     */

    template<class TPhy,template <class> class... Features>
    inline void MouseHidDevice<TPhy,Features...>::onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event) {

      event.descriptor=reinterpret_cast<uint8_t *>(&_qualifierDescriptor);
      event.length=sizeof(_qualifierDescriptor);
    }


    /**
     * Data in event
     */

    template<class TPhy,template <class> class... Features>
    inline void MouseHidDevice<TPhy,Features...>::onHidDataIn() {

      // clear the busy flag

      this->_busy=false;
    }


    /**
      * USBD_HID_Setup. Handle the HID specific requests
      * @param pdev: instance
      * @param req: usb requests
      * @retval status
      */

    template<class TPhy,template <class> class... Features>
    inline void MouseHidDevice<TPhy,Features...>::onHidSetup(DeviceClassSdkSetupEvent& event) {

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
                             const_cast<uint8_t *>(MouseReportDescriptor),
                             Min<uint16_t>(sizeof(MouseReportDescriptor),event.request.wLength));

          } else if(event.request.wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {

            USBD_CtlSendData(&this->_deviceHandle,
                             reinterpret_cast<uint8_t *>(&_mouseDescriptor.hid),
                             Min<uint16_t>(sizeof(_mouseDescriptor.hid),event.request.wLength));
          }

          break;

        case USB_REQ_GET_INTERFACE:
          USBD_CtlSendData(&this->_deviceHandle,(uint8_t *)&this->_hidAltSetting,1);
          break;

        case USB_REQ_SET_INTERFACE:
          this->_hidAltSetting=event.request.wValue;
          break;
      }
    }


    /**
     * Send a 3-byte HID report to the host
     * @param data The data to send
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool MouseHidDevice<TPhy,Features...>::mouseSendReport(uint8_t buttons,int8_t x,int8_t y) {

      uint8_t data[3];

      data[0]=buttons;
      data[1]=x;
      data[2]=y;

      return HidDeviceBase::hidSendReport(
          static_cast<const MouseHidDeviceEndpoint1<Device<TPhy>>&>(*this),
          data,
          MOUSE_HID_REPORT_SIZE);
    }
  }
}
