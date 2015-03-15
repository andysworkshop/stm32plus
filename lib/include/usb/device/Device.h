/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /*
     * USB Device template. This is an intermediate template in the hierarchy. Inherits from UsbDeviceEventSource
     * to provide event subscription for device-specific events. Subclasses such as UsbHidDevice inherit from
     * this template to provide device-class specific functionality.
     */

    template<class TPhy>
    class Device : public Core<TPhy> {

      protected:
        PCD_HandleTypeDef _pcdHandle;
        USBD_HandleTypeDef _deviceHandle;
        USBD_DescriptorsTypeDef _deviceDescriptorCallbacks;
        scoped_ptr<LanguageDescriptor> _languageDescriptor;
        DeviceDescriptor _deviceDescriptor;
        DeviceQualifierDescriptor _qualifierDescriptor;
        scoped_array<uint16_t> _unicodeString;      // very basic 8-bit to UTF-16 holder
        uint8_t _numEndpoints;                      // endpoint classes will increment this

      public:

        /**
         * Error codes
         */

        enum {
          E_INIT = 1,
          E_START = 2,
          E_UNCONFIGURED = 3,
          E_BUSY = 4
        };


        /**
         * Parameters structure - device specific detail
         */

        struct Parameters : Core<TPhy>::Parameters {

          uint16_t device_vid;          // no default
          uint16_t device_pid;          // no default
          uint16_t device_usb_version;  // defaults to 0x0200 (USB 2.0)
          uint16_t device_version;      // your device version. default 01.00
          uint16_t device_id;           // this device id (default is 0)
          const uint16_t *device_language_ids;  // default is 0x0409
          uint16_t device_language_count;       // default is 1
          bool device_use_dedicated_endpoint1;  // default is false
          bool device_vbus_sensing;     // default is true
          uint32_t device_endpoint0_max_packet_size;  // default is 0x40

          Parameters() {

            static const uint16_t englishUs=0x0409;

            device_usb_version=0x0200;
            device_version=0x0100;
            device_id=0;
            device_use_dedicated_endpoint1=false;
            device_endpoint0_max_packet_size=0x40;
            device_vbus_sensing=true;
            device_language_ids=&englishUs;
            device_language_count=1;
          }
        };

        static Device<TPhy> *_instance;    // this is how the global callbacks get back in

      protected:
        void onEvent(UsbEventDescriptor& event);
        void onResetIrqEvent();
        void onGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event);

      public:
        Device();
        ~Device();

        bool initialise(Parameters& params);

        bool isConfigured() const;

        USBD_HandleTypeDef& getDeviceHandle();
        DeviceDescriptor& getDeviceDescriptor();
        PCD_HandleTypeDef& getPcdHandle();

        void incrementNumEndpoints();

        // re-entry points from the SDK

        uint8_t *onGetDeviceDescriptor(USBD_SpeedTypeDef speed,uint16_t *length);
        uint8_t *onGetLangIdStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length);
        uint8_t *onGetDisplayStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length,uint8_t stringIndex);
    };


    /*
     * Template static member initialisation
     */

    template<class TPhy>
    Device<TPhy> *Device<TPhy>::_instance=nullptr;


    /*
     * Internal global callbacks needed by the SDK device API
     */

    namespace usb_device_internal {

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetDeviceDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDeviceDescriptor(speed,length);
      }

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetLangIdStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetLangIdStrDescriptor(speed,length);
      }

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetManufacturerStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_MFC_STR);
      }

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetProductStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_PRODUCT_STR);
      }

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetSerialStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_SERIAL_STR);
      }

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetConfigurationStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_CONFIG_STR);
      }

      template<class TPhy>
      __attribute__((noinline)) inline uint8_t *GetInterfaceStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_INTERFACE_STR);
      }
    }


    /**
     * Constructor
     * @param params The parameters structure
     */

    template<class TPhy>
    inline Device<TPhy>::Device()
      : _pcdHandle(),
        _deviceHandle() {

      // static member initialisation

      _instance=this;

      // reset the endpoint counter

      _numEndpoints=0;

      // this is how the "LL" callbacks raise events

      _deviceHandle.pUserData=static_cast<UsbEventSource *>(this);

      // fill in the SDK device descriptor callbacks

      _deviceDescriptorCallbacks.GetDeviceDescriptor=usb_device_internal::GetDeviceDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetLangIDStrDescriptor=usb_device_internal::GetLangIdStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetManufacturerStrDescriptor=usb_device_internal::GetManufacturerStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetProductStrDescriptor=usb_device_internal::GetProductStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetSerialStrDescriptor=usb_device_internal::GetSerialStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetConfigurationStrDescriptor=usb_device_internal::GetConfigurationStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetInterfaceStrDescriptor=usb_device_internal::GetInterfaceStrDescriptor<TPhy>;

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&Device<TPhy>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param params The parameters
     * @return true if it worked
     */

    template<class TPhy>
    bool Device<TPhy>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!Core<TPhy>::initialise(params))
        return false;

      // set up the device descriptor

      _deviceDescriptor.bcdDevice=params.device_version;
      _deviceDescriptor.bcdUSB=params.device_usb_version;
      _deviceDescriptor.idVendor=params.device_vid;
      _deviceDescriptor.idProduct=params.device_pid;

      // set up the language descriptor

      _languageDescriptor.reset(new LanguageDescriptor(params.device_language_ids,params.device_language_count));

      // set up the PCD handle

      _pcdHandle.Instance=this->_phyRegisters;

      _pcdHandle.Init.dev_endpoints=_numEndpoints;
      _pcdHandle.Init.use_dedicated_ep1=params.device_use_dedicated_endpoint1 ? ENABLE : DISABLE;
      _pcdHandle.Init.ep0_mps=params.device_endpoint0_max_packet_size;
      _pcdHandle.Init.vbus_sensing_enable=params.device_vbus_sensing ? ENABLE : DISABLE;
      _pcdHandle.Init.dma_enable=this->phySupportsDma();
      _pcdHandle.Init.low_power_enable=this->phySupportsLowPower() ? ENABLE : DISABLE;
      _pcdHandle.Init.phy_itface=this->getPhyInterface();
      _pcdHandle.Init.Sof_enable=0;
      _pcdHandle.Init.speed=this->getPhySpeed();

      // link the driver to the stack

      _pcdHandle.pData=&_deviceHandle;
      _deviceHandle.pData=&_pcdHandle;

      // initialise SDK

      if((status=USBD_Init(&_deviceHandle,&_deviceDescriptorCallbacks,params.device_id))!=USBD_OK)
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_USB_DEVICE,E_INIT,status);

      // both FS and HS PHYs manage a shared RX FIFO

      HAL_PCD_SetRxFiFo(&_pcdHandle,params.phy_rxFifoSize);
      return true;
    }


    /**
     * Check if this device is configured, i.e. connected to the host and fully
     * ready to use. Note that there is a race condition if you call this as a pre-check to
     * sending a report. Better to send the report and check the return value, ignoring errors
     * that indicate unconfigured.
     * @return true if the device is configured and ready to use
     */

    template<class TPhy>
    inline bool Device<TPhy>::isConfigured() const {
      return _deviceHandle.dev_state==USBD_STATE_CONFIGURED;
    }


    /**
     * Destructor
     */

    template<class TPhy>
    inline Device<TPhy>::~Device() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&Device<TPhy>::onEvent)
        );
    }


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TPhy>
    __attribute__((noinline)) inline void Device<TPhy>::onEvent(UsbEventDescriptor& event) {

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::OTG_FS_INTERRUPT:
          HAL_PCD_IRQHandler(&_pcdHandle);    // handle the OTG interrupt
          break;

        case UsbEventDescriptor::EventType::DEVICE_INIT:
          static_cast<DeviceSdkNotifyEvent&>(event).retval=HAL_PCD_Init(&_pcdHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_DEINIT:
          static_cast<DeviceSdkNotifyEvent&>(event).retval=HAL_PCD_DeInit(&_pcdHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_SET_USB_ADDRESS:
          static_cast<DeviceSdkNotifyEvent&>(event).retval=HAL_PCD_SetAddress(&_pcdHandle,_deviceHandle.dev_address);
          break;

        case UsbEventDescriptor::EventType::DEVICE_START:
          static_cast<DeviceSdkNotifyEvent&>(event).retval=HAL_PCD_Start(&_pcdHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_STOP:
          static_cast<DeviceSdkNotifyEvent&>(event).retval=HAL_PCD_Stop(&_pcdHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_OPEN_ENDPOINT: {
            DeviceSdkOpenEndpointEvent& openEvent(static_cast<DeviceSdkOpenEndpointEvent&>(event));
            openEvent.retval=HAL_PCD_EP_Open(&_pcdHandle,openEvent.endpointAddress,openEvent.maximumPacketSize,static_cast<uint8_t>(openEvent.endpointType));
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_CLOSE_ENDPOINT: {
            DeviceSdkEndpointEvent& epEvent(static_cast<DeviceSdkEndpointEvent&>(event));
            epEvent.retval=HAL_PCD_EP_Close(&_pcdHandle,epEvent.endpointAddress);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_FLUSH_ENDPOINT: {
            DeviceSdkEndpointEvent& epEvent(static_cast<DeviceSdkEndpointEvent&>(event));
            epEvent.retval=HAL_PCD_EP_Flush(&_pcdHandle,epEvent.endpointAddress);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_STALL_ENDPOINT: {
            DeviceSdkEndpointEvent& epEvent(static_cast<DeviceSdkEndpointEvent&>(event));
            epEvent.retval=HAL_PCD_EP_SetStall(&_pcdHandle,epEvent.endpointAddress);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_CLEAR_STALL_ENDPOINT: {
            DeviceSdkEndpointEvent& epEvent(static_cast<DeviceSdkEndpointEvent&>(event));
            epEvent.retval=HAL_PCD_EP_ClrStall(&_pcdHandle,epEvent.endpointAddress);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_IS_STALL_ENDPOINT: {
            DeviceSdkIsStalledEndpointEvent& epEvent(static_cast<DeviceSdkIsStalledEndpointEvent&>(event));

            if((epEvent.endpointAddress & 0x80)==0x80)
              epEvent.isStalled=_pcdHandle.IN_ep[epEvent.endpointAddress & 0x7F].is_stall;
            else
              epEvent.isStalled=_pcdHandle.OUT_ep[epEvent.endpointAddress & 0x7F].is_stall;
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_TRANSMIT: {
            DeviceSdkDataEndpointEvent& dataEvent(static_cast<DeviceSdkDataEndpointEvent&>(event));
            dataEvent.retval=HAL_PCD_EP_Transmit(&_pcdHandle,dataEvent.endpointAddress,dataEvent.data,dataEvent.size);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_PREPARE_RECEIVE: {
            DeviceSdkDataEndpointEvent& dataEvent(static_cast<DeviceSdkDataEndpointEvent&>(event));
            dataEvent.retval=HAL_PCD_EP_Receive(&_pcdHandle,dataEvent.endpointAddress,dataEvent.data,dataEvent.size);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_GET_RX_DATA_SIZE: {
            DeviceSdkGetLastTransferredSizeEndpointEvent& dataEvent(static_cast<DeviceSdkGetLastTransferredSizeEndpointEvent&>(event));
            dataEvent.lastTransferred=HAL_PCD_EP_GetRxCount(&_pcdHandle,dataEvent.endpointAddress);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_RESET:
          onResetIrqEvent();
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_SUSPEND:
          USBD_LL_Suspend(&_deviceHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_RESUME:
          USBD_LL_Resume(&_deviceHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_SETUP_STAGE:
          USBD_LL_SetupStage(&_deviceHandle,reinterpret_cast<uint8_t *>(_pcdHandle.Setup));
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_CONNECT:
          USBD_LL_DevConnected(&_deviceHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_DISCONNECT:
          USBD_LL_DevDisconnected(&_deviceHandle);
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_DATA_IN_STAGE: {
            uint8_t endpointNumber=static_cast<DeviceSdkDataInStageInterruptEvent&>(event).endpointNumber;
            USBD_LL_DataInStage(&_deviceHandle,endpointNumber,_pcdHandle.IN_ep[endpointNumber].xfer_buff);
          }
          break;

        case UsbEventDescriptor::EventType::DEVICE_IRQ_DATA_OUT_STAGE: {
            uint8_t endpointNumber=static_cast<DeviceSdkDataOutStageInterruptEvent&>(event).endpointNumber;
            USBD_LL_DataOutStage(&_deviceHandle,endpointNumber,_pcdHandle.OUT_ep[endpointNumber].xfer_buff);
          }
          break;

        case UsbEventDescriptor::EventType::CLASS_GET_DEVICE_QUALIFIER_DESCRIPTOR:
          onGetDeviceQualifierDescriptor(static_cast<DeviceClassSdkGetDeviceQualifierDescriptorEvent&>(event));
          break;

        default:
          break;
      }
    }


    /**
     * Get the device qualifier descriptor
     * @param event The event class to receive the descriptor pointer
     */

    template<class TPhy>
    inline void Device<TPhy>::onGetDeviceQualifierDescriptor(DeviceClassSdkGetDeviceQualifierDescriptorEvent& event) {

      event.descriptor=&_qualifierDescriptor;
      event.length=sizeof(_qualifierDescriptor);
    }


    /**
     * USB reset IRQ handler
     */

    template<class TPhy>
    inline void Device<TPhy>::onResetIrqEvent() {

      USBD_SpeedTypeDef speed;

      switch(_pcdHandle.Init.speed) {

        case PCD_SPEED_HIGH:
          speed = USBD_SPEED_HIGH;
          break;

        case PCD_SPEED_FULL:
          speed=USBD_SPEED_FULL;
          break;

        default:
          speed=USBD_SPEED_FULL;
          break;
      }

      // set the speed and reset

      USBD_LL_SetSpeed(&_deviceHandle,speed);
      USBD_LL_Reset(&_deviceHandle);
    }


    /**
     * Get the USB device descriptor (SDK callback). This happens when connected. An event is raised that
     * allows the caller to modify the device descriptor dynamically based on the speed of the connection
     * @param speed The connection speed
     * @param length Will get set to 18
     * @return The device descriptor pointer
     */

    template<class TPhy>
    inline uint8_t *Device<TPhy>::onGetDeviceDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {

      // give anyone a chance to change the descriptor based on the connection speed

      this->UsbEventSender.raiseEvent(DeviceGetDeviceDescriptorEvent(speed,_deviceDescriptor));

      // return the descriptor

      *length=USB_LEN_DEV_DESC;
      return reinterpret_cast<uint8_t *>(&_deviceDescriptor);
    }


    /**
     * Get the USB language descriptor (SDK callback). This happens when connected. An event is raised that
     * allows the caller to modify the language descriptor dynamically.
     * @param speed The connection speed
     * @param length Will get set to 18
     * @return The device descriptor pointer
     */

    template<class TPhy>
    inline uint8_t *Device<TPhy>::onGetLangIdStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {

      // give anyone a chance to change the descriptor based on the connection speed

      this->UsbEventSender.raiseEvent(DeviceGetLanguageDescriptorEvent(speed,*_languageDescriptor));

      // return the descriptor

      *length=_languageDescriptor->getDescriptor()[0];
      return _languageDescriptor->getDescriptor();
    }


    /**
     * Get the USB language descriptor (SDK callback). This happens when connected. An event is raised that
     * allows the caller to modify the language descriptor dynamically.
     * @param speed The connection speed
     * @param length Will get set to 18
     * @return The device descriptor pointer
     */

    template<class TPhy>
    inline uint8_t *Device<TPhy>::onGetDisplayStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length,uint8_t stringIndex) {

      DeviceGetDisplayStringDescriptorEvent event(speed,stringIndex);

      // send the event that will be picked up by one of the feature classes that implements
      // the required string

      this->UsbEventSender.raiseEvent(event);

      // subscriber sets these, or if not that's OK too

      *length=event.length;
      return event.descriptor;
    }


    /**
     * Get the device handle
     * @return a reference to the SDK device handle
     */

    template<class TPhy>
    inline USBD_HandleTypeDef& Device<TPhy>::getDeviceHandle() {
      return _deviceHandle;
    }


    /**
     * Get the PCD handle
     * @return a reference to the SDK PCD handle
     */

    template<class TPhy>
    inline PCD_HandleTypeDef& Device<TPhy>::getPcdHandle() {
      return _pcdHandle;
    }


    /**
     * Increment the number of endpoints by 1
     */

    template<class TPhy>
    inline void Device<TPhy>::incrementNumEndpoints() {
      _numEndpoints++;
    }


    /**
     * Get the device descriptor reference
     * @return The device descriptor
     */

    template<class TPhy>
    inline DeviceDescriptor& Device<TPhy>::getDeviceDescriptor() {
      return _deviceDescriptor;
    }


    namespace usb_device_internal {

      /*
       * Helpers to dispatch events that have common parameters
       */

      inline USBD_StatusTypeDef notifyEvent(USBD_HandleTypeDef *pdev,UsbEventDescriptor::EventType eventType) {

        DeviceSdkNotifyEvent event(eventType);

        reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
        return event.retval;
      }

      inline USBD_StatusTypeDef endpointEvent(USBD_HandleTypeDef *pdev,UsbEventDescriptor::EventType eventType,uint8_t ep_addr) {

        DeviceSdkEndpointEvent event(eventType,ep_addr);

        reinterpret_cast<UsbEventSource *>(pdev->pUserData)->UsbEventSender.raiseEvent(event);
        return event.retval;
      }
    }
  }
}
