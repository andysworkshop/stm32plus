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
     * Derivation of HidDevice to handle a HID keyboard. This device type declares the following:
     *   1x Configuration descriptor
     *   1x Interface
     *   1x Inbound interrupt endpoint (key presses inbound to the host)
     *   1x Outbound interrupt endpoint (LED status outbound from the host)
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

           uint8_t hid_keyboard_in_poll_interval;        // default is 10
           uint8_t hid_keyboard_out_poll_interval;       // default is 10

           Parameters() {
             hid_keyboard_in_poll_interval=10;
             hid_keyboard_out_poll_interval=10;
           }
         };

         static KeyboardHidDevice<TPhy,Features...> *_instance;    // this is how the global callbacks get back in


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

      public:
        KeyboardHidDevice();

        bool initialise(Parameters& params);

        bool keyboardSendReport(const uint8_t *data);

        uint8_t onHidInit(uint8_t cfgindx);
        uint8_t onHidDeInit(uint8_t cfgindx);
        uint8_t onHidSetup(USBD_SetupReqTypedef *req);
        uint8_t *onHidGetConfigurationDescriptor(uint16_t *length);
        uint8_t *onHidGetDeviceQualifierDescriptor(uint16_t *length);
        uint8_t onHidDataIn(uint8_t epnum);
        uint8_t onHidDataOut(uint8_t epnum);
        uint8_t onHidEp0RxReady();
    };


    /*
     * Template static member initialisation
     */

    template<class TPhy,template <class> class... Features>
    KeyboardHidDevice<TPhy,Features...> *KeyboardHidDevice<TPhy,Features...>::_instance=nullptr;


    /**
     * Internal SDK callbacks
     */

    namespace keyboard_hid_device_internal {

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_Init(USBD_HandleTypeDef */* pdev */,uint8_t cfgidx) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidInit(cfgidx);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_DeInit(USBD_HandleTypeDef */* pdev */,uint8_t cfgidx) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidDeInit(cfgidx);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_Setup(USBD_HandleTypeDef * /* pdev */,USBD_SetupReqTypedef *req) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidSetup(req);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t *HID_GetCfgDesc(uint16_t *length) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidGetConfigurationDescriptor(length);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t *HID_GetDeviceQualifierDesc(uint16_t *length) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidGetDeviceQualifierDescriptor(length);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_DataIn(USBD_HandleTypeDef * /* pdev */,uint8_t epnum) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidDataIn(epnum);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_DataOut(USBD_HandleTypeDef * /* pdev */,uint8_t epnum) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidDataOut(epnum);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_Ep0RxReady(USBD_HandleTypeDef * /* pdev */) {
        return KeyboardHidDevice<TPhy,Features...>::_instance->onHidEp0RxReady();
      }
    }


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline KeyboardHidDevice<TPhy,Features...>::KeyboardHidDevice() {

      // static member initialisation

      _instance=this;
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

      _keyboardDescriptor.outEndpoint.bEndpointAddress=EndpointDescriptor::OUT | 1;
      _keyboardDescriptor.outEndpoint.bmAttributes=EndpointDescriptor::INTERRUPT;
      _keyboardDescriptor.outEndpoint.wMaxPacketSize=KEYBOARD_HID_LED_REPORT_SIZE;         // LED reports are 6 bytes
      _keyboardDescriptor.outEndpoint.bInterval=params.hid_keyboard_out_poll_interval;     // default is 10ms

      // set up the qualifier descriptor (see constructor for defaults)

      _qualifierDescriptor.bDeviceClass=0;
      _qualifierDescriptor.bDeviceSubClass=0;
      _qualifierDescriptor.bDeviceProtocol=0;
      _qualifierDescriptor.bMaxPacketSize0=0x40;
      _qualifierDescriptor.bNumConfigurations=1;

      // set up the SDK class type

      this->_classType.Init=keyboard_hid_device_internal::HID_Init<TPhy,Features...>;
      this->_classType.DeInit=keyboard_hid_device_internal::HID_DeInit<TPhy,Features...>;
      this->_classType.Setup=keyboard_hid_device_internal::HID_Setup<TPhy,Features...>;
      this->_classType.DataIn=keyboard_hid_device_internal::HID_DataIn<TPhy,Features...>;
      this->_classType.DataOut=keyboard_hid_device_internal::HID_DataOut<TPhy,Features...>;
      this->_classType.EP0_RxReady=keyboard_hid_device_internal::HID_Ep0RxReady<TPhy,Features...>;
      this->_classType.GetHSConfigDescriptor=keyboard_hid_device_internal::HID_GetCfgDesc<TPhy,Features...>;
      this->_classType.GetFSConfigDescriptor=keyboard_hid_device_internal::HID_GetCfgDesc<TPhy,Features...>;
      this->_classType.GetOtherSpeedConfigDescriptor=keyboard_hid_device_internal::HID_GetCfgDesc<TPhy,Features...>;
      this->_classType.GetDeviceQualifierDescriptor=keyboard_hid_device_internal::HID_GetDeviceQualifierDesc<TPhy,Features...>;

      // link the HID interface/endpoint registration into the SDK structure

      if((status=USBD_RegisterClass(&this->_deviceHandle,&this->_classType))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,this->E_REGISTER_CLASS,status);

      // start the device

      if((status=USBD_Start(&this->_deviceHandle))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,this->E_START,status);

      // OK

      return true;
    }


    /**
     * HID initialisation
     */

    template<class TPhy,template <class> class... Features>
    inline uint8_t KeyboardHidDevice<TPhy,Features...>::onHidInit(uint8_t cfgindx) {

      USBD_LL_OpenEP(&this->_deviceHandle,EndpointDescriptor::IN | 1,EndpointDescriptor::INTERRUPT,KEYBOARD_HID_KEYS_REPORT_SIZE);
      USBD_LL_OpenEP(&this->_deviceHandle,EndpointDescriptor::OUT | 2,EndpointDescriptor::INTERRUPT,KEYBOARD_HID_LED_REPORT_SIZE);

      // device is not busy

      this->_busy=false;

      // prepare OUT endpoint to receive the first packet

      USBD_LL_PrepareReceive(&this->_deviceHandle,EndpointDescriptor::OUT | 2,_outReportBuffer,sizeof(_outReportBuffer));

      HidSdkInitEvent event(cfgindx);;
      this->UsbEventSender.raiseEvent(event);

      return event.status;
    }


    /**
     * De-initialise the HID device
     * @param pdev
     * @param cfgidx
     * @return
     */

    template<class TPhy,template <class> class... Features>
    inline uint8_t KeyboardHidDevice<TPhy,Features...>::onHidDeInit(uint8_t cfgidx) {

      // close endpoints

      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::IN | 1);
      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::OUT | 2);

      HidSdkDeInitEvent event(cfgidx);
      this->UsbEventSender.raiseEvent(event);

      return event.status;
    }


    /**
     * @brief  USBD_HID_DataIn
     *         handle data IN Stage
     * @param  pdev: device instance
     * @param  epnum: endpoint index
     * @retval status
     */

    template<class TPhy,template <class> class... Features>
    inline uint8_t *KeyboardHidDevice<TPhy,Features...>::onHidGetConfigurationDescriptor(uint16_t *length) {

      // create the event with the default descriptor and send

      HidSdkGetConfigurationDescriptorEvent event(reinterpret_cast<uint8_t *>(&_keyboardDescriptor),sizeof(_keyboardDescriptor));
      this->UsbEventSender.raiseEvent(event);

      // set the return values

      *length=event.length;
      return event.descriptor;
    }


    /**
     * @brief  DeviceQualifierDescriptor
     *         return Device Qualifier descriptor
     * @param  length : pointer data length
     * @retval pointer to descriptor buffer
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline))  inline uint8_t *KeyboardHidDevice<TPhy,Features...>::onHidGetDeviceQualifierDescriptor(uint16_t *length) {

      // create the event with the default descriptor and send

      HidSdkGetDeviceQualifierDescriptorEvent event(_qualifierDescriptor);
      this->UsbEventSender.raiseEvent(event);

      // set the return values

      *length=sizeof(DeviceQualifierDescriptor);
      return reinterpret_cast<uint8_t *>(&_qualifierDescriptor);
    }


    /**
     * Data in event
     * @param epnum endpoint number
     * @return
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline))  inline uint8_t KeyboardHidDevice<TPhy,Features...>::onHidDataIn(uint8_t epnum) {

      // Ensure that the FIFO is empty before a new transfer, this condition could
      // be caused by  a new transfer before the end of the previous transfer

      this->_busy=false;

      // create the event with the default descriptor and send

      HidSdkDataInEvent event(epnum);
      this->UsbEventSender.raiseEvent(event);

      return event.status;
    }


    /**
     * Data out event
     * @param epnum endpoint number
     * @return
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline))  inline uint8_t KeyboardHidDevice<TPhy,Features...>::onHidDataOut(uint8_t epnum) {

      // create the event with the default descriptor and send

      HidSdkDataOutEvent event(epnum,_outReportBuffer,sizeof(_outReportBuffer));
      this->UsbEventSender.raiseEvent(event);

      USBD_LL_PrepareReceive(
          &this->_deviceHandle,
          EndpointDescriptor::OUT | 2,
          _outReportBuffer,
          sizeof(_outReportBuffer));

      return event.status;
    }


    /**
     * Control endpoint ready handler
     * @return
     */

    template<class TPhy,template <class> class... Features>
    __attribute__((noinline))  inline uint8_t KeyboardHidDevice<TPhy,Features...>::onHidEp0RxReady() {

      // has a report arrived?

      if(this->_isReportAvailable) {

        // send the data event

        HidSdkDataOutEvent event(EndpointDescriptor::OUT | 2,_outReportBuffer,sizeof(_outReportBuffer));
        this->UsbEventSender.raiseEvent(event);

        // clear the available flag

        this->_isReportAvailable=false;
      }

      // send the notify event

      this->UsbEventSender.raiseEvent(HidSdkEp0ReadyEvent());
      return USBD_OK;
    }


    /**
      * USBD_HID_Setup. Handle the HID specific requests
      * @param pdev: instance
      * @param req: usb requests
      * @retval status
      */

    template<class TPhy,template <class> class... Features>
    inline uint8_t KeyboardHidDevice<TPhy,Features...>::onHidSetup(USBD_SetupReqTypedef *req) {

      uint16_t len;
      uint8_t *pbuf;

      // send the event

      HidSdkSetupEvent event(*req);
      this->UsbEventSender.raiseEvent(event);

      // check for fail

      if(event.status!=USBD_OK)
        return event.status;

      // process the setup event handled here

      if((req->bmRequest & USB_REQ_TYPE_MASK)!=USB_REQ_TYPE_STANDARD)
        return USBD_OK;

      switch(req->bRequest) {

        case USB_REQ_GET_DESCRIPTOR:
          if(req->wValue >> 8 == HidClassDescriptor::HID_REPORT_DESCRIPTOR) {
            len=Min<uint16_t>(sizeof(KeyboardReportDescriptor),req->wLength);
            pbuf=const_cast<uint8_t *>(KeyboardReportDescriptor);
          } else if(req->wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {
            pbuf=reinterpret_cast<uint8_t *>(&_keyboardDescriptor.hid);
            len=Min<uint16_t>(sizeof(_keyboardDescriptor.hid),req->wLength);
          }

          USBD_CtlSendData(&this->_deviceHandle,pbuf,len);
          break;

        case USB_REQ_GET_INTERFACE:
          USBD_CtlSendData(&this->_deviceHandle,&this->_hidAltSetting,1);
          break;

        case USB_REQ_SET_INTERFACE:
          this->_hidAltSetting=req->wValue;
          break;
      }

      return USBD_OK;
    }


    /**
     * Send an 8-byte HID report to the host. The data must be an 8-byte standard
     * keyboard report (modifiers, reserved, 6-key rollover).
     * @param data The data to send.
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool KeyboardHidDevice<TPhy,Features...>::keyboardSendReport(const uint8_t *data) {

      return HidDeviceBase::hidSendReport(
          static_cast<const KeyboardHidDeviceKeysEndpoint<Device<TPhy>>&>(*this),
          data,
          KEYBOARD_HID_KEYS_REPORT_SIZE);
    }
  }
}
