/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Endpoints used by this device. The interrupt IN endpoint for notifying the host
     * is supplied by the parent class
     */

    template<class TDevice> using ComPortCdcDeviceDataInEndpoint=BulkInEndpointFeature<1,TDevice>;
    template<class TDevice> using ComPortCdcDeviceDataOutEndpoint=BulkOutEndpointFeature<1,TDevice>;


    /**
     * Declare the structure that gets sent back when the host asks for the whole
     * configuration descriptor
     */

    struct ComPortCdcConfigurationDescriptor {
      ConfigurationDescriptor configuration;
      InterfaceAssociationDescriptor assoc;

      InterfaceDescriptor itf0;
      CdcHeaderFunctionalDescriptor cdcHeader;
      CdcCallManagementFunctionalDescriptor cdcCallManagement;
      CdcAbstractControlManagementFunctionalDescriptor cdcAcm;
      CdcOneSlaveUnionFunctionalDescriptor cdcUnion;
      EndpointDescriptor commandEndpoint;

      InterfaceDescriptor itf1;
      EndpointDescriptor outEndpoint;
      EndpointDescriptor inEndpoint;

    } __attribute__((packed));


    /**
     * Derivation of CdcDevice to provide a CDC virtual COM port device.
     *
     *   1x Configuration descriptor
     *   2x Interfaces
     *   1x Inbound bulk data endpoint
     *   1x Outbound bulk data endpoint
     *   1x Inbound command endpoint
     */

     template<class TPhy,template <class> class... Features>
     class ComPortCdcDevice : public CdcDevice<TPhy,
                                               ComPortCdcConfigurationDescriptor,
                                               ComPortCdcDeviceDataInEndpoint,
                                               ComPortCdcDeviceDataOutEndpoint,
                                               Features...> {

       public:

         typedef CdcDevice<TPhy,
                           ComPortCdcConfigurationDescriptor,
                           ComPortCdcDeviceDataInEndpoint,
                           ComPortCdcDeviceDataOutEndpoint,
                           Features...> CdcDeviceBase;

         /**
          * Customisable parameters for this CDC device
          */

         struct Parameters : CdcDeviceBase::Parameters {

           uint16_t cdc_com_port_in_max_packet_size;       // default is 64 bytes
           uint16_t cdc_com_port_out_max_packet_size;      // default is 64 bytes
           uint16_t cdc_com_port_rx_buffer_size;           // default is 1024

           Parameters() {
             cdc_com_port_in_max_packet_size=64;
             cdc_com_port_out_max_packet_size=64;
             cdc_com_port_rx_buffer_size=1024;
           }
         };

       protected:

         enum {
           DATA_IN_EP_ADDRESS = EndpointDescriptor::IN | 1,     // data in endpoint address
           DATA_OUT_EP_ADDRESS = EndpointDescriptor::OUT | 1,   // data out endpoint address
         };

         uint16_t _maxInPacketSize;
         uint16_t _maxOutPacketSize;
         scoped_array<uint8_t> _rxBuffer;
         uint16_t _rxBufferSize;

      protected:
        void onEvent(UsbEventDescriptor& event);

        void onCdcInit();
        void onCdcDeInit();
        void onCdcGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event);
        void onCdcDataOut(DeviceClassSdkDataOutEvent& event);
        void onCdcEp0RxReady();

      public:
        ComPortCdcDevice();
        ~ComPortCdcDevice();

        bool initialise(Parameters& params);

        bool transmit(const void *data,uint16_t len);
        bool isTransmittingData() const;
        void beginReceive();
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline ComPortCdcDevice<TPhy,Features...>::ComPortCdcDevice() {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&ComPortCdcDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline ComPortCdcDevice<TPhy,Features...>::~ComPortCdcDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&ComPortCdcDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool ComPortCdcDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!CdcDeviceBase::initialise(params))
        return false;

      // create TX/RX buffers

      _rxBuffer.reset(new uint8_t[params.cdc_com_port_rx_buffer_size]);

      // remember some params

      _maxInPacketSize=params.cdc_com_port_in_max_packet_size;
      _maxOutPacketSize=params.cdc_com_port_out_max_packet_size;
      _rxBufferSize=params.cdc_com_port_rx_buffer_size;

      // set up the configuration descriptor (see constructor for defaults)

      this->_configurationDescriptor.configuration.wTotalLength=sizeof(this->_configurationDescriptor);
      this->_configurationDescriptor.configuration.bNumInterfaces=2;
      this->_configurationDescriptor.configuration.bmAttributes=0x80 | ConfigurationDescriptor::SELF_POWERED;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,ComPortCdcDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface association descriptor

      this->_configurationDescriptor.assoc.bFirstInterface=0;
      this->_configurationDescriptor.assoc.bInterfaceCount=2;
      this->_configurationDescriptor.assoc.bFunctionClass=2;
      this->_configurationDescriptor.assoc.bFunctionSubClass=2;
      this->_configurationDescriptor.assoc.bFunctionProtocol=1;
      this->_configurationDescriptor.assoc.iFunction=0;

      // set up interface descriptor 0 (see constructor for defaults)

      this->_configurationDescriptor.itf0.bInterfaceNumber=0;
      this->_configurationDescriptor.itf0.bNumEndpoints=1;
      this->_configurationDescriptor.itf0.bInterfaceClass=static_cast<uint8_t>(DeviceClass::COMMUNICATIONS_CDC_CONTROL);
      this->_configurationDescriptor.itf0.bInterfaceSubClass=static_cast<uint8_t>(CdcSubClass::ABSTRACT_CONTROL_MODEL);
      this->_configurationDescriptor.itf0.bInterfaceProtocol=static_cast<uint8_t>(CdcProtocol::COMMON_AT_COMMANDS);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,ComPortCdcDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.itf0.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the call management functional descriptor

      this->_configurationDescriptor.cdcCallManagement.bmCapabilities=0;
      this->_configurationDescriptor.cdcCallManagement.bDataInterface=1;      // interface #1 does the data

      // set up the ACM functional descriptor

      this->_configurationDescriptor.cdcAcm.bmCapabilities=0x02;

      // set up the union functional descriptor

      this->_configurationDescriptor.cdcUnion.bMasterInterface=0;
      this->_configurationDescriptor.cdcUnion.bSlaveInterface=1;

      // set up interface descriptor 1 (see constructor for defaults)

      this->_configurationDescriptor.itf1.bInterfaceNumber=1;
      this->_configurationDescriptor.itf1.bNumEndpoints=2;
      this->_configurationDescriptor.itf1.bInterfaceClass=static_cast<uint8_t>(DeviceClass::CDC_DATA);
      this->_configurationDescriptor.itf1.bInterfaceSubClass=0;
      this->_configurationDescriptor.itf1.bInterfaceProtocol=0;

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,ComPortCdcDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.itf1.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the OUT endpoint descriptor

      this->_configurationDescriptor.outEndpoint.bEndpointAddress=DATA_OUT_EP_ADDRESS;
      this->_configurationDescriptor.outEndpoint.bmAttributes=EndpointDescriptor::BULK;
      this->_configurationDescriptor.outEndpoint.wMaxPacketSize=params.cdc_com_port_out_max_packet_size;         // LED reports are 1 byte

      // set up the IN endpoint descriptor

      this->_configurationDescriptor.inEndpoint.bEndpointAddress=DATA_IN_EP_ADDRESS;
      this->_configurationDescriptor.inEndpoint.bmAttributes=EndpointDescriptor::BULK;
      this->_configurationDescriptor.inEndpoint.wMaxPacketSize=params.cdc_com_port_in_max_packet_size;

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
    __attribute__((noinline)) inline void ComPortCdcDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_INIT:
          onCdcInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          onCdcDeInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DATA_OUT:
          onCdcDataOut(static_cast<DeviceClassSdkDataOutEvent&>(event));
          break;

        case UsbEventDescriptor::EventType::CLASS_EP0_READY:
          onCdcEp0RxReady();
          break;

        case UsbEventDescriptor::EventType::CLASS_GET_CONFIGURATION_DESCRIPTOR:
          onCdcGetConfigurationDescriptor(static_cast<DeviceClassSdkGetConfigurationDescriptorEvent&>(event));
          break;

        default:
          break;
      }
    }


    /**
     * CDC initialisation. Opens the data endpoints and initiates the first beginReceive() call.
     */

    template<class TPhy,template <class> class... Features>
    inline void ComPortCdcDevice<TPhy,Features...>::onCdcInit() {

      // reset state

      this->_opCode=CdcControlCommand::NONE;

      // open data endpoints

      USBD_LL_OpenEP(&this->_deviceHandle,DATA_IN_EP_ADDRESS,EndpointDescriptor::BULK,_maxInPacketSize);
      USBD_LL_OpenEP(&this->_deviceHandle,DATA_OUT_EP_ADDRESS,EndpointDescriptor::BULK,_maxOutPacketSize);

      // prepare OUT endpoint to receive the first packet

      beginReceive();
    }


    /**
     * De-initialise the CDC device
     */

    template<class TPhy,template <class> class... Features>
    inline void ComPortCdcDevice<TPhy,Features...>::onCdcDeInit() {

      // close data endpoints

      USBD_LL_CloseEP(&this->_deviceHandle,DATA_IN_EP_ADDRESS);
      USBD_LL_CloseEP(&this->_deviceHandle,DATA_OUT_EP_ADDRESS);
    }


    /**
     * Get the configuration descriptor
     * @param event The event class to receive the descriptor and provide type of descriptor being requested
     */

    template<class TPhy,template <class> class... Features>
    inline void ComPortCdcDevice<TPhy,Features...>::onCdcGetConfigurationDescriptor(DeviceClassSdkGetConfigurationDescriptorEvent& event) {

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
    inline void ComPortCdcDevice<TPhy,Features...>::onCdcDataOut(DeviceClassSdkDataOutEvent& event) {

      uint32_t size;

      // get the received size

      size=USBD_LL_GetRxDataSize(&this->_deviceHandle,event.endpointNumber);

      // notify the application code

      this->UsbEventSender.raiseEvent(CdcDataReceivedEvent(_rxBuffer.get(),size));
    }


    /**
     * Control endpoint ready handler
     */

    template<class TPhy,template <class> class... Features>
    inline void ComPortCdcDevice<TPhy,Features...>::onCdcEp0RxReady() {

      if(this->_opCode!=CdcControlCommand::NONE) {

        // notify the application

        this->UsbEventSender.raiseEvent(CdcControlEvent(this->_opCode,this->_commandBuffer.get(),this->_commandSize));
        this->_opCode=CdcControlCommand::NONE;
      }
    }


    /**
     * Transmit some data. This is a blocking call. It will wait until any previous
     * transmission is complete before sending the data. 'data' must remain in scope
     * until the transmission has completed.
     * @param data The data buffer to send
     * @param len The size of the data buffer
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool ComPortCdcDevice<TPhy,Features...>::transmit(const void *data,uint16_t len) {

      ComPortCdcDeviceDataInEndpoint<Device<TPhy>>& endpoint=static_cast<ComPortCdcDeviceDataInEndpoint<Device<TPhy>>&>(*this);

      // wait for previous send to complete by IRQ notification

      while(endpoint.isTransmitting());

      // send this data

      return endpoint.transmit(data,len);
    }


    /**
     * Check if the bulk IN endpoint is transmitting
     * @return true if the endpoint is transmitting
     */

    template<class TPhy,template <class> class... Features>
    inline bool ComPortCdcDevice<TPhy,Features...>::isTransmittingData() const {
      return static_cast<const ComPortCdcDeviceDataInEndpoint<Device<TPhy>>&>(*this).isTransmitting();
    }


    /**
     * Prepare to receive the next packet. This must be called after each packet has been
     * received and processed to initiate the reception of the next packet.
     */

    template<class TPhy,template <class> class... Features>
    inline void ComPortCdcDevice<TPhy,Features...>::beginReceive() {

      USBD_LL_PrepareReceive(
          &this->_deviceHandle,
          DATA_OUT_EP_ADDRESS,
          _rxBuffer.get(),
          _rxBufferSize);
    }
  }
}
