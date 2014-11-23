/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Endpoints used by this device
     */

    template<class TDevice> using KeyboardHidDeviceKeysEndpoint=InterruptInEndpointFeature<1,TDevice>;
    template<class TDevice> using KeyboardHidDeviceLedsEndpoint=InterruptOutEndpointFeature<2,TDevice>;

    /**
     * Derivation of HidDevice to handle a HID keyboard using the BOOT protocol. This device type
     * declares the following:
     *
     *   1x Configuration descriptor
     *   1x Interface
     *   1x Inbound interrupt endpoint (key presses inbound to the host)
     *   1x Outbound interrupt endpoint (LED status outbound from the host)
     *
     * The 8-byte report sent to the host has the following format:
     *
     *          Bit7      Bit6      Bit5        Bit4       Bit3     Bit2     Bit1       Bit0
     *   Byte0  Right GUI Right Alt Right Shift Right Ctrl Left GUI Left Alt Left Shift Left Ctrl
     *   Byte1                                     Reserved
     *   Byte2                                   Key_array[0]
     *   Byte3                                   Key_array[1]
     *   Byte4                                   Key_array[2]
     *   Byte5                                   Key_array[3]
     *   Byte6                                   Key_array[4]
     *   Byte7                                   Key_array[5]
     *
     * The 1-byte sent by the host to this device has the following format:
     *
     *         Bit7  Bit6  Bit5  Bit4  Bit3     Bit2        Bit1       Bit0
     *   Byte0                   Kana  Compose  Scroll Lock Caps Lock  Num Lock
     */

    template<class TPhy,template <class> class... Features>
    class KeyboardHidDevice : public HidDevice<TPhy,
                                               KeyboardHidDeviceKeysEndpoint,
                                               KeyboardHidDeviceLedsEndpoint,
                                               Features...> {

       public:

         typedef HidDevice<TPhy,KeyboardHidDeviceKeysEndpoint,KeyboardHidDeviceLedsEndpoint,Features...> HidDeviceBase;

         /**
          * Constants
          */

         enum {
           KEYBOARD_HID_KEYS_REPORT_SIZE = 8,    // must be 8 to be accepted by BOOT protocol
           KEYBOARD_HID_LED_REPORT_SIZE = 1
         };


         /**
          * Customisable parameters for this HID device
          */

         struct Parameters : HidDeviceBase::Parameters {

           uint8_t hid_keyboard_in_poll_interval;     // default is 10
           uint8_t hid_keyboard_out_poll_interval;    // default is 10

           Parameters() {
             hid_keyboard_in_poll_interval=10;
             hid_keyboard_out_poll_interval=10;
           }
         };

      protected:

        /**
         * Declare the structure that gets sent back when the host asks for the whole
         * configuration descriptor
         */

        struct KeyboardConfigurationDescriptor {
          ConfigurationDescriptor configuration;
          InterfaceDescriptor interface;
          HidClassDescriptor hid;
          EndpointDescriptor inEndpoint;
          EndpointDescriptor outEndpoint;
        } __attribute__((packed));

        KeyboardConfigurationDescriptor _keyboardDescriptor;
        DeviceQualifierDescriptor _qualifierDescriptor;

        uint8_t _outReportBuffer[KEYBOARD_HID_LED_REPORT_SIZE];

      protected:
        void onEvent(UsbEventDescriptor& event);

        void onHidInit();
        void onHidDeInit();
        void onHidSetup(DeviceClassSdkSetupEvent& event);
        void onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event);
        void onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event);
        void onHidDataIn();
        void onHidDataOut();
        void onHidEp0RxReady();

      public:
        KeyboardHidDevice();
        ~KeyboardHidDevice();

        bool initialise(Parameters& params);

        bool keyboardSendKeyReport(uint8_t key,uint8_t modifiers=0);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline KeyboardHidDevice<TPhy,Features...>::KeyboardHidDevice() {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&KeyboardHidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline KeyboardHidDevice<TPhy,Features...>::~KeyboardHidDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&KeyboardHidDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool KeyboardHidDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!HidDeviceBase::initialise(params))
        return false;

      // set up the configuration descriptor (see constructor for defaults)

      _keyboardDescriptor.configuration.wTotalLength=sizeof(_keyboardDescriptor);
      _keyboardDescriptor.configuration.bNumInterfaces=1;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,KeyboardHidDevice<TPhy,Features...>>::value)
        _keyboardDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor (see constructor for defaults)

      _keyboardDescriptor.interface.bInterfaceNumber=0;
      _keyboardDescriptor.interface.bNumEndpoints=2;
      _keyboardDescriptor.interface.bInterfaceClass=static_cast<uint8_t>(DeviceClass::HID);
      _keyboardDescriptor.interface.bInterfaceSubClass=static_cast<uint8_t>(HidSubClass::BOOT);
      _keyboardDescriptor.interface.bInterfaceProtocol=static_cast<uint8_t>(HidProtocol::KEYBOARD);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,KeyboardHidDevice<TPhy,Features...>>::value)
        _keyboardDescriptor.interface.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the hid class descriptor (see constructor for defaults)

      _keyboardDescriptor.hid.bNumDescriptors=1;
      _keyboardDescriptor.hid.wItemLength=sizeof(KeyboardReportDescriptor);

      // set up the IN endpoint descriptor

      _keyboardDescriptor.inEndpoint.bEndpointAddress=EndpointDescriptor::IN | 1;
      _keyboardDescriptor.inEndpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      _keyboardDescriptor.inEndpoint.wMaxPacketSize=KEYBOARD_HID_KEYS_REPORT_SIZE;    // key reports are 8 bytes
      _keyboardDescriptor.inEndpoint.bInterval=params.hid_keyboard_in_poll_interval;  // default is 10ms

      // set up the OUT endpoint descriptor

      _keyboardDescriptor.outEndpoint.bEndpointAddress=EndpointDescriptor::OUT | 2;
      _keyboardDescriptor.outEndpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      _keyboardDescriptor.outEndpoint.wMaxPacketSize=KEYBOARD_HID_LED_REPORT_SIZE;         // LED reports are 1 byte
      _keyboardDescriptor.outEndpoint.bInterval=params.hid_keyboard_out_poll_interval;     // default is 10ms

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
    __attribute__((noinline)) inline void KeyboardHidDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

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

        case UsbEventDescriptor::EventType::CLASS_DATA_OUT:
          onHidDataOut();
          break;

        case UsbEventDescriptor::EventType::CLASS_EP0_READY:
          onHidEp0RxReady();
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
    inline void KeyboardHidDevice<TPhy,Features...>::onHidInit() {

      USBD_LL_OpenEP(&this->_deviceHandle,EndpointDescriptor::IN | 1,EndpointDescriptor::INTERRUPT,KEYBOARD_HID_KEYS_REPORT_SIZE);
      USBD_LL_OpenEP(&this->_deviceHandle,EndpointDescriptor::OUT | 2,EndpointDescriptor::INTERRUPT,KEYBOARD_HID_LED_REPORT_SIZE);

      // device is not busy

      this->_busy=false;

      // prepare OUT endpoint to receive the first packet

      USBD_LL_PrepareReceive(&this->_deviceHandle,EndpointDescriptor::OUT | 2,_outReportBuffer,sizeof(_outReportBuffer));
    }


    /**
     * De-initialise the HID device
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidDeInit() {

      // close endpoints

      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::IN | 1);
      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::OUT | 2);
    }


    /**
     * Get the configuration descriptor
     * @param event The event class to receive the descriptor and provide type of descriptor being requested
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event) {

      if(event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::HIGH_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::FULL_SPEED ||
         event.type==DeviceClassSdkGetConfigurationDescriptorEvent::Type::OTHER_SPEED) {

        // set up the values in the event

        event.length=sizeof(_keyboardDescriptor);
        event.descriptor=reinterpret_cast<uint8_t *>(&_keyboardDescriptor);
      }
    }


    /**
     * Get the device qualifier descriptor
     * @param event The event class to receive the descriptor pointer
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event) {

      event.descriptor=reinterpret_cast<uint8_t *>(&_qualifierDescriptor);
      event.length=sizeof(_qualifierDescriptor);
    }


    /**
     * Data in event
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidDataIn() {

      // not busy any more

      this->_busy=false;
    }


    /**
     * Data out event
     * @param epnum endpoint number
     * @return
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidDataOut() {

      // call the prepare-receive callback

      USBD_LL_PrepareReceive(
          &this->_deviceHandle,
          EndpointDescriptor::OUT | 2,
          _outReportBuffer,
          sizeof(_outReportBuffer));

      // fire the LED state event

      this->UsbEventSender.raiseEvent(HidKeyboardLedStateEvent(_outReportBuffer[0]));
    }


    /**
     * Control endpoint ready handler
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidEp0RxReady() {

      // has a report arrived?

      if(this->_isReportAvailable) {

        // send the data event

        DeviceClassSdkDataOutEvent event(EndpointDescriptor::OUT | 2);
        this->UsbEventSender.raiseEvent(event);

        // clear the available flag

        this->_isReportAvailable=false;
      }
    }


    /**
      * USBD_HID_Setup. Handle the HID specific requests
      * @param pdev: instance
      * @param req: usb requests
      * @retval status
      */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidSetup(DeviceClassSdkSetupEvent& event) {

      // check for fail

      if(event.status!=USBD_OK)
        return;

      // process the setup event handled here

      if((event.request.bmRequest & USB_REQ_TYPE_MASK)==USB_REQ_TYPE_CLASS) {

        switch(static_cast<HidClassRequestType>(event.request.bRequest)) {

          case HidClassRequestType::SET_REPORT:
            this->_isReportAvailable=true;
            USBD_CtlPrepareRx(&this->_deviceHandle,_outReportBuffer,event.request.wLength);
            break;

          case HidClassRequestType::GET_REPORT:
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
                               const_cast<uint8_t *>(KeyboardReportDescriptor),
                               Min<uint16_t>(sizeof(KeyboardReportDescriptor),event.request.wLength));

            } else if(event.request.wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {

              USBD_CtlSendData(&this->_deviceHandle,
                               reinterpret_cast<uint8_t *>(&_keyboardDescriptor.hid),
                               Min<uint16_t>(sizeof(_keyboardDescriptor.hid),event.request.wLength));

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
    }


    /**
     * Send a single keycode to the host with optional modifiers. The keycode is a USB keycode,
     * not an ASCII character.
     * send the zero report that indicates the key has been released
     * @param key The key to send
     * @param modifiers The state of the modifier keys
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool KeyboardHidDevice<TPhy,Features...>::keyboardSendKeyReport(uint8_t key,uint8_t modifiers) {

      uint8_t report[KEYBOARD_HID_KEYS_REPORT_SIZE];

      report[1]=report[3]=report[4]=report[5]=report[6]=report[7]='\0';
      report[0]=modifiers;
      report[2]=key;

      // send the actual report

      if(!HidDeviceBase::hidSendReport(
          static_cast<const KeyboardHidDeviceKeysEndpoint<Device<TPhy>>&>(*this),
          report,
          KEYBOARD_HID_KEYS_REPORT_SIZE))
        return false;

      // send the zero report

      report[0]=report[2]='\0';

      // wait for the IRQ to reset the busy flag

      while(this->_busy);

      return HidDeviceBase::hidSendReport(
          static_cast<const KeyboardHidDeviceKeysEndpoint<Device<TPhy>>&>(*this),
          report,
          KEYBOARD_HID_KEYS_REPORT_SIZE);
    }
  }
}
