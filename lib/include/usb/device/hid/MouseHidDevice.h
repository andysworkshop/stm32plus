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

        static MouseHidDevice<TPhy,Features...> *_instance;    // this is how the global callbacks get back in

      public:
        MouseHidDevice();

        bool initialise(Parameters& params);

        bool mouseSendReport(uint8_t buttons,int8_t x,int8_t y);

        uint8_t onHidInit(uint8_t cfgindx);
        uint8_t onHidDeInit(uint8_t cfgindx);
        uint8_t onHidSetup(USBD_SetupReqTypedef *req);
        uint8_t *onHidGetConfigurationDescriptor(uint16_t *length);
        uint8_t *onHidGetDeviceQualifierDescriptor(uint16_t *length);
        uint8_t onHidDataIn(uint8_t epnum);
    };


    /*
     * Template static member initialisation
     */

    template<class TPhy,template <class> class... Features>
    MouseHidDevice<TPhy,Features...> *MouseHidDevice<TPhy,Features...>::_instance=nullptr;


    /**
     * Internal SDK callbacks
     */

    namespace mouse_hid_device_internal {

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_Init(USBD_HandleTypeDef */* pdev */,uint8_t cfgidx) {
        return MouseHidDevice<TPhy,Features...>::_instance->onHidInit(cfgidx);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_DeInit(USBD_HandleTypeDef */* pdev */,uint8_t cfgidx) {
        return MouseHidDevice<TPhy,Features...>::_instance->onHidDeInit(cfgidx);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_Setup(USBD_HandleTypeDef * /* pdev */,USBD_SetupReqTypedef *req) {
        return MouseHidDevice<TPhy,Features...>::_instance->onHidSetup(req);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t *HID_GetCfgDesc(uint16_t *length) {
        return MouseHidDevice<TPhy,Features...>::_instance->onHidGetConfigurationDescriptor(length);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t *HID_GetDeviceQualifierDesc(uint16_t *length) {
        return MouseHidDevice<TPhy,Features...>::_instance->onHidGetDeviceQualifierDescriptor(length);
      }

      template<class TPhy,template <class> class... Features>
      __attribute__((noinline)) inline uint8_t HID_DataIn(USBD_HandleTypeDef * /* pdev */,uint8_t epnum) {
        return MouseHidDevice<TPhy,Features...>::_instance->onHidDataIn(epnum);
      }
    }


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline MouseHidDevice<TPhy,Features...>::MouseHidDevice() {

      // static member initialisation

      _instance=this;
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

      // set up the SDK class type

      this->_classType.Init=mouse_hid_device_internal::HID_Init<TPhy,Features...>;
      this->_classType.DeInit=mouse_hid_device_internal::HID_DeInit<TPhy,Features...>;
      this->_classType.Setup=mouse_hid_device_internal::HID_Setup<TPhy,Features...>;
      this->_classType.DataIn=mouse_hid_device_internal::HID_DataIn<TPhy,Features...>;
      this->_classType.GetHSConfigDescriptor=mouse_hid_device_internal::HID_GetCfgDesc<TPhy,Features...>;
      this->_classType.GetFSConfigDescriptor=mouse_hid_device_internal::HID_GetCfgDesc<TPhy,Features...>;
      this->_classType.GetOtherSpeedConfigDescriptor=mouse_hid_device_internal::HID_GetCfgDesc<TPhy,Features...>;
      this->_classType.GetDeviceQualifierDescriptor=mouse_hid_device_internal::HID_GetDeviceQualifierDesc<TPhy,Features...>;

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
    inline uint8_t MouseHidDevice<TPhy,Features...>::onHidInit(uint8_t cfgindx) {

      USBD_LL_OpenEP(&this->_deviceHandle,EndpointDescriptor::IN | 1,EndpointDescriptor::INTERRUPT,MOUSE_HID_REPORT_SIZE);

      // device is not busy

      this->_busy=false;

      // send init event

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
    inline uint8_t MouseHidDevice<TPhy,Features...>::onHidDeInit(uint8_t cfgidx) {

      // close the endpoint

      USBD_LL_CloseEP(&this->_deviceHandle,EndpointDescriptor::IN | 1);

      // send the notification

      HidSdkDeInitEvent event(cfgidx);
      this->UsbEventSender.raiseEvent(event);

      return event.status;
    }


    /**
     * Get the configuration descriptor
     * @param  pdev: device instance
     * @param  epnum: endpoint index
     * @retval status
     */

    template<class TPhy,template <class> class... Features>
    inline uint8_t *MouseHidDevice<TPhy,Features...>::onHidGetConfigurationDescriptor(uint16_t *length) {

      // create the event with the default descriptor and send

      HidSdkGetConfigurationDescriptorEvent event(reinterpret_cast<uint8_t *>(&_mouseDescriptor),sizeof(_mouseDescriptor));
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
    __attribute__((noinline))  inline uint8_t *MouseHidDevice<TPhy,Features...>::onHidGetDeviceQualifierDescriptor(uint16_t *length) {

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
    __attribute__((noinline))  inline uint8_t MouseHidDevice<TPhy,Features...>::onHidDataIn(uint8_t epnum) {

      // Ensure that the FIFO is empty before a new transfer, this condition could
      // be caused by  a new transfer before the end of the previous transfer

      this->_busy=false;

      // create the event with the default descriptor and send

      HidSdkDataInEvent event(epnum);
      this->UsbEventSender.raiseEvent(event);

      return event.status;
    }


    /**
      * USBD_HID_Setup. Handle the HID specific requests
      * @param pdev: instance
      * @param req: usb requests
      * @retval status
      */

    template<class TPhy,template <class> class... Features>
    inline uint8_t MouseHidDevice<TPhy,Features...>::onHidSetup(USBD_SetupReqTypedef *req) {

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
            len=Min<uint16_t>(sizeof(MouseReportDescriptor),req->wLength);
            pbuf=const_cast<uint8_t *>(MouseReportDescriptor);
          } else if(req->wValue >> 8 == HidClassDescriptor::HID_DESCRIPTOR_TYPE) {
            pbuf=reinterpret_cast<uint8_t *>(&_mouseDescriptor.hid);
            len=Min<uint16_t>(sizeof(_mouseDescriptor.hid),req->wLength);
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
