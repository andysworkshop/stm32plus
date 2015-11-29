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
                                               KeyboardConfigurationDescriptor,
                                               KeyboardHidDeviceKeysEndpoint,
                                               KeyboardHidDeviceLedsEndpoint,
                                               Features...> {

       public:

         typedef HidDevice<TPhy,
                           KeyboardConfigurationDescriptor,
                           KeyboardHidDeviceKeysEndpoint,
                           KeyboardHidDeviceLedsEndpoint,
                           Features...> HidDeviceBase;

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
        uint8_t _outReportBuffer[KEYBOARD_HID_LED_REPORT_SIZE];

      protected:
        void onEvent(UsbEventDescriptor& event);

        void onHidInit();
        void onHidDeInit();
        void onHidSetup(DeviceClassSdkSetupEvent& event);
        void onHidGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event);
        void onHidDataOut();
        void onHidEp0RxReady();

      public:
        KeyboardHidDevice();
        ~KeyboardHidDevice();

        bool initialise(Parameters& params);

        bool sendKeyboardReport(uint8_t key,uint8_t modifiers=0);
        bool isTransmitting() const;
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline KeyboardHidDevice<TPhy,Features...>::KeyboardHidDevice()
      : HidDeviceBase(KeyboardReportDescriptor,sizeof(KeyboardReportDescriptor)) {

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

      this->_configurationDescriptor.configuration.wTotalLength=sizeof(this->_configurationDescriptor);
      this->_configurationDescriptor.configuration.bNumInterfaces=1;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,KeyboardHidDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor (see constructor for defaults)

      this->_configurationDescriptor.interface.bInterfaceNumber=0;
      this->_configurationDescriptor.interface.bNumEndpoints=2;
      this->_configurationDescriptor.interface.bInterfaceClass=static_cast<uint8_t>(DeviceClass::HID);
      this->_configurationDescriptor.interface.bInterfaceSubClass=static_cast<uint8_t>(HidSubClass::BOOT);
      this->_configurationDescriptor.interface.bInterfaceProtocol=static_cast<uint8_t>(HidProtocol::KEYBOARD);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,KeyboardHidDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.interface.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the hid class descriptor (see constructor for defaults)

      this->_configurationDescriptor.hid.bNumDescriptors=1;
      this->_configurationDescriptor.hid.wItemLength=sizeof(KeyboardReportDescriptor);

      // set up the IN endpoint descriptor

      this->_configurationDescriptor.inEndpoint.bEndpointAddress=EndpointDescriptor::IN | 1;
      this->_configurationDescriptor.inEndpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      this->_configurationDescriptor.inEndpoint.wMaxPacketSize=KEYBOARD_HID_KEYS_REPORT_SIZE;    // key reports are 8 bytes
      this->_configurationDescriptor.inEndpoint.bInterval=params.hid_keyboard_in_poll_interval;  // default is 10ms

      // set up the OUT endpoint descriptor

      this->_configurationDescriptor.outEndpoint.bEndpointAddress=EndpointDescriptor::OUT | 2;
      this->_configurationDescriptor.outEndpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      this->_configurationDescriptor.outEndpoint.wMaxPacketSize=KEYBOARD_HID_LED_REPORT_SIZE;         // LED reports are 1 byte
      this->_configurationDescriptor.outEndpoint.bInterval=params.hid_keyboard_out_poll_interval;     // default is 10ms

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
    __attribute__((noinline)) inline void KeyboardHidDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_INIT:
          onHidInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          onHidDeInit();
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

        event.length=sizeof(this->_configurationDescriptor);
        event.descriptor=reinterpret_cast<uint8_t *>(&this->_configurationDescriptor);
      }
    }


    /**
     * Data out event
     */

    template<class TPhy,template <class> class... Features>
    inline void KeyboardHidDevice<TPhy,Features...>::onHidDataOut() {

      // fire the LED state event

      this->UsbEventSender.raiseEvent(HidKeyboardLedStateEvent(_outReportBuffer[0]));

      // call the prepare-receive method to initiate the next read

      USBD_LL_PrepareReceive(
          &this->_deviceHandle,
          EndpointDescriptor::OUT | 2,
          _outReportBuffer,
          sizeof(_outReportBuffer));
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
     * Handle the HID setup requests
     * @param event the event containg value being requested
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
    inline bool KeyboardHidDevice<TPhy,Features...>::sendKeyboardReport(uint8_t key,uint8_t modifiers) {

      // data must remain in scope until IRQ indicates transmission complete

      static uint8_t report[KEYBOARD_HID_KEYS_REPORT_SIZE];

      report[1]=report[3]=report[4]=report[5]=report[6]=report[7]='\0';
      report[0]=modifiers;
      report[2]=key;

      KeyboardHidDeviceKeysEndpoint<Device<TPhy>>& endpoint=static_cast<KeyboardHidDeviceKeysEndpoint<Device<TPhy>>&>(*this);

      // wait for previous send to complete by IRQ notification

      while(isTransmitting());

      // send the actual report

      if(!endpoint.transmit(report,KEYBOARD_HID_KEYS_REPORT_SIZE))
        return false;

      // send the zero report

      report[0]=report[2]='\0';

      // wait for the IRQ to reset the busy flag

      while(isTransmitting());

      // send the zero report (key up)

      return endpoint.transmit(report,KEYBOARD_HID_KEYS_REPORT_SIZE);
    }


    /**
     * Check if is transmitting
     * @return true if is transmitting
     */

    template<class TPhy,template <class> class... Features>
    inline bool KeyboardHidDevice<TPhy,Features...>::isTransmitting() const {
      return static_cast<const KeyboardHidDeviceKeysEndpoint<Device<TPhy>>&>(*this).isTransmitting();
    }
  }
}
