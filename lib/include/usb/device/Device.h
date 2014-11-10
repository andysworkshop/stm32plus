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
    class Device : public DeviceBase<TPhy> {

      protected:
        PCD_HandleTypeDef _pcdHandle;
        USBD_HandleTypeDef _deviceHandle;
        USBD_DescriptorsTypeDef _deviceDescriptorCallbacks;
        DeviceDescriptor _deviceDescriptor;
        LanguageDescriptor _languageDescriptor;
        scoped_array<uint16_t> _unicodeString;      // very basic 8-bit to UTF-16 holder
        uint8_t _numEndpoints;                      // endpoint classes will increment this

      public:

        /**
         * Parameters structure - device specific detail
         */

        struct Parameters : DeviceBase<TPhy>::Parameters {

          uint16_t device_vid;          // no default
          uint16_t device_pid;          // no default
          uint16_t device_usb_version;  // defaults to 0x0200 (USB 2.0)
          uint16_t device_version;      // your device version. default 01.00
          uint16_t device_id;           // this device id (default is 0)
          uint16_t device_language_id;  // default is 0x0409 (English; US)
          bool device_use_dedicated_endpoint1;  // default is false
          bool device_vbus_sensing;     // default is true
          uint32_t device_endpoint0_max_packet_size;  // default is 0x40

          Parameters() {
            device_usb_version=0x0200;
            device_version=0x0100;
            device_id=0;
            device_language_id=0x0409;
            device_use_dedicated_endpoint1=false;
            device_endpoint0_max_packet_size=0x40;
            device_vbus_sensing=true;
          }
        };

        static Device<TPhy> *_instance;    // this is how the global callbacks get back in

      protected:
        void onEvent(DeviceEventDescriptor& event);

      public:
        Device(Parameters& params);
        ~Device();

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
      inline uint8_t *GetDeviceDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDeviceDescriptor(speed,length);
      }

      template<class TPhy>
      inline uint8_t *GetLangIdStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetLangIdStrDescriptor(speed,length);
      }

      template<class TPhy>
      inline uint8_t *GetManufacturerStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_MFC_STR);
      }

      template<class TPhy>
      inline uint8_t *GetProductStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_PRODUCT_STR);
      }

      template<class TPhy>
      inline uint8_t *GetSerialStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_SERIAL_STR);
      }

      template<class TPhy>
      inline uint8_t *GetConfigurationStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_CONFIG_STR);
      }

      template<class TPhy>
      inline uint8_t *GetInterfaceStrDescriptor(USBD_SpeedTypeDef speed,uint16_t *length) {
        return Device<TPhy>::_instance->onGetDisplayStrDescriptor(speed,length,USBD_IDX_INTERFACE_STR);
      }
    }


    /**
     * Constructor
     * @param params The parameters structure
     */

    template<class TPhy>
    inline Device<TPhy>::Device(Parameters& params)
      : DeviceBase<TPhy>(params) {

      // static member initialisation

      _instance=this;

      // reset the endpoint counter

      _numEndpoints=0;

      // this is how the "LL" callbacks raise events

      _deviceHandle.pUserData=static_cast<DeviceEventSource *>(this);

      // fill in the SDK device descriptor callbacks

      _deviceDescriptorCallbacks.GetDeviceDescriptor=usb_device_internal::GetDeviceDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetLangIDStrDescriptor=usb_device_internal::GetLangIdStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetManufacturerStrDescriptor=usb_device_internal::GetManufacturerStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetProductStrDescriptor=usb_device_internal::GetProductStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetSerialStrDescriptor=usb_device_internal::GetSerialStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetConfigurationStrDescriptor=usb_device_internal::GetConfigurationStrDescriptor<TPhy>;
      _deviceDescriptorCallbacks.GetInterfaceStrDescriptor=usb_device_internal::GetInterfaceStrDescriptor<TPhy>;

      // set up the device descriptor

      _deviceDescriptor.bcdDevice=params.device_version;
      _deviceDescriptor.bcdUSB=params.device_usb_version;

      // set up the language descriptor

      _languageDescriptor.wLanguageId=params.device_language_id;

       // subscribe to USB events

      this->DeviceEventSender.insertSubscriber(
          DeviceEventSourceSlot::bind(this,&Device<TPhy>::onEvent)
        );

      // initialise SDK

      USBD_Init(&_deviceHandle,&_deviceDescriptorCallbacks,params.device_id);

      // set up the PCD handle

      _pcdHandle.Instance=this->_phyRegisters;

      _pcdHandle.Init.dev_endpoints=_numEndpoints;
      _pcdHandle.Init.use_dedicated_ep1=params.device_use_dedicated_endpoint1;
      _pcdHandle.Init.ep0_mps=params.device_endpoint0_max_packet_size;
      _pcdHandle.Init.vbus_sensing_enable=params.device_vbus_sensing;
      _pcdHandle.Init.low_power_enable=params.device_low_power_enable;

      _pcdHandle.Init.dma_enable=this->phySupportsDma();
      _pcdHandle.Init.phy_itface=this->getPhyInterface();
      _pcdHandle.Init.Sof_enable=0;
      _pcdHandle.Init.speed=this->getPhySpeed();

      // link the driver to the stack

      _pcdHandle.pData=&_deviceHandle;
      _deviceHandle.pData=&_pcdHandle;

      // initialise the HAL driver

      HAL_PCD_Init(&_pcdHandle);

      // both FS and HS PHYs manage a shared RX FIFO

      HAL_PCD_SetRxFiFo(&_pcdHandle,params.phy_rxFifoSize);
   }


    /**
     * Destructor
     */

    template<class TPhy>
    inline Device<TPhy>::~Device() {

      // unsubscribe from USB events

      this->DeviceEventSender.removeSubscriber(
          DeviceEventSourceSlot::bind(this,&Device<TPhy>::onEvent)
        );
    }


    /**
     * Event handler for device events
     * @param event The event descriptor
     */

    template<class TPhy>
    inline void Device<TPhy>::onEvent(DeviceEventDescriptor& /* event */) {

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

      this->DeviceEventSender.raiseEvent(DeviceGetDeviceDescriptorEvent(speed,_deviceDescriptor));

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

      this->DeviceEventSender.raiseEvent(DeviceGetLanguageDescriptorEvent(speed,_languageDescriptor));

      // return the descriptor

      *length=USB_LEN_LANGID_STR_DESC;
      return reinterpret_cast<uint8_t *>(&_languageDescriptor);
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

      this->DeviceEventSender.raiseEvent(event);

      // subscriber sets these, or if not that's OK too

      *length=event.length;
      return reinterpret_cast<uint8_t *>(event.string);
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

      inline USBD_StatusTypeDef notifyEvent(USBD_HandleTypeDef *pdev,DeviceEventDescriptor::EventType eventType) {

        DeviceSdkNotifyEvent event(eventType);

        reinterpret_cast<DeviceEventSource *>(pdev->pUserData)->DeviceEventSender.raiseEvent(event);
        return event.retval;
      }

      inline USBD_StatusTypeDef endpointEvent(USBD_HandleTypeDef *pdev,DeviceEventDescriptor::EventType eventType,uint8_t ep_addr) {

        DeviceSdkEndpointEvent event(eventType,ep_addr);

        reinterpret_cast<DeviceEventSource *>(pdev->pUserData)->DeviceEventSender.raiseEvent(event);
        return event.retval;
      }
    }
  }
}
