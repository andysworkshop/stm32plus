/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace usb {

    /**
     * Endpoints used by this device. The Bulk Only Transport has one bulk IN and one OUT
     * endpoint.
     */

    template<class TDevice> using BotMscDeviceInEndpoint=BulkInEndpointFeature<1,TDevice>;
    template<class TDevice> using BotMscDeviceOutEndpoint=BulkOutEndpointFeature<2,TDevice>;


    /**
     * Declare the structure that gets sent back when the host asks for the whole
     * configuration descriptor
     */

    struct BotMscDeviceConfigurationDescriptor {
      ConfigurationDescriptor configuration;
      InterfaceDescriptor itf;
      EndpointDescriptor inEndpoint;
      EndpointDescriptor outEndpoint;
    } __attribute__((packed));


    /**
     * Derivation of MscDevice to provide support for the Bulk Only Transport (BOT). BOT
     * is the most common transport used in USB sticks and USB hard disks. The protocol
     * implements a limited SCSI command subset.
     *
     *   1x Configuration descriptor
     *   2x Interfaces
     *   1x Inbound bulk data endpoint
     *   1x Outbound bulk data endpoint
     */

     template<class TPhy,template <class> class... Features>
     class BotMscDevice : public MscDevice<TPhy,
                                           BotMscDeviceConfigurationDescriptor,
                                           BotMscDeviceInEndpoint,
                                           BotMscDeviceOutEndpoint,
                                           Features...> {

       public:

         typedef MscDevice<TPhy,
                           BotMscDeviceConfigurationDescriptor,
                           BotMscDeviceInEndpoint,
                           BotMscDeviceOutEndpoint,
                           Features...> MscDeviceBase;

         /*
          * Endpoint addresses
          */

         enum {
           IN_EP_ADDRESS = EndpointDescriptor::IN | 1,     // IN endpoint address
           OUT_EP_ADDRESS = EndpointDescriptor::OUT | 2,   // OUT endpoint address
         };


         /**
          * Customisable parameters for this MSC BOT device
          */

         struct Parameters : MscDeviceBase::Parameters, MscScsi<IN_EP_ADDRESS,OUT_EP_ADDRESS>::Parameters {

           uint16_t msc_bot_max_packet_size;        // default is 64 bytes

           Parameters() {
             msc_bot_max_packet_size=0x40;
           }
         };

       protected:

         uint8_t _interface;
         uint8_t _maxLun;
         MscBotState _state;
         MscBotStatus _status;
         MscBotCommandBlockWrapper _cbw;
         MscBotCommandStatusWrapper _csw;
         MscScsi<IN_EP_ADDRESS,OUT_EP_ADDRESS> _scsi;

      protected:
        void onEvent(UsbEventDescriptor& event);

        void onInit();
        void onDeInit();
        void onSetup(DeviceClassSdkSetupEvent& event);
        void onSetupClass(DeviceClassSdkSetupEvent& event);
        void onSetupInterfaceAndEndpoint(DeviceClassSdkSetupEvent& event);
        void onGetMaxLun(DeviceClassSdkSetupEvent& event);
        void onBotReset(DeviceClassSdkSetupEvent& event);
        void onClearFeature(DeviceClassSdkSetupEvent& event);
        void onDataIn();
        void onDataOut();

        void decodeCbw();
        void abortTransfer();
        void sendData(uint8_t *buf,uint16_t len);

      public:
        BotMscDevice();
        ~BotMscDevice();

        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline BotMscDevice<TPhy,Features...>::BotMscDevice()
      : _scsi(_state,static_cast<UsbEventSource&>(*this),this->_deviceHandle) {

      // subscribe to USB events

      this->UsbEventSender.insertSubscriber(
          UsbEventSourceSlot::bind(this,&BotMscDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Destructor
     */

    template<class TPhy,template <class> class... Features>
    inline BotMscDevice<TPhy,Features...>::~BotMscDevice() {

      // unsubscribe from USB events

      this->UsbEventSender.removeSubscriber(
          UsbEventSourceSlot::bind(this,&BotMscDevice<TPhy,Features...>::onEvent)
        );
    }


    /**
     * Initialise the class
     * @param param The parameters structure
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool BotMscDevice<TPhy,Features...>::initialise(Parameters& params) {

      USBD_StatusTypeDef status;

      // initialise upwards

      if(!MscDeviceBase::initialise(params) || !_scsi.initialise(params))
        return false;

      // set up the configuration descriptor (see constructor for defaults)

      this->_configurationDescriptor.configuration.wTotalLength=sizeof(this->_configurationDescriptor);
      this->_configurationDescriptor.configuration.bNumInterfaces=1;
      this->_configurationDescriptor.configuration.bmAttributes=0x80 | ConfigurationDescriptor::SELF_POWERED;

      // if ConfigurationTextFeature is in the hierarchy then we've got a configuration string (compile-time check)

      if(std::is_base_of<ConfigurationTextFeature<Device<TPhy>>,BotMscDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.configuration.iConfiguration=USBD_IDX_CONFIG_STR;

      // set up the interface descriptor

      this->_configurationDescriptor.itf.bInterfaceNumber=0;
      this->_configurationDescriptor.itf.bNumEndpoints=2;
      this->_configurationDescriptor.itf.bInterfaceClass=static_cast<uint8_t>(DeviceClass::MASS_STORAGE);
      this->_configurationDescriptor.itf.bInterfaceSubClass=static_cast<uint8_t>(MscSubClass::SCSI_TRANSPARENT);
      this->_configurationDescriptor.itf.bInterfaceProtocol=static_cast<uint8_t>(MscProtocol::BOT);

      // if InterfaceTextFeature is in the hierarchy then we've got an interface string (compile-time check)

      if(std::is_base_of<InterfaceTextFeature<Device<TPhy>>,BotMscDevice<TPhy,Features...>>::value)
        this->_configurationDescriptor.itf.iInterface=USBD_IDX_INTERFACE_STR;

      // set up the OUT endpoint descriptor

      this->_configurationDescriptor.outEndpoint.bEndpointAddress=OUT_EP_ADDRESS;
      this->_configurationDescriptor.outEndpoint.bmAttributes=EndpointDescriptor::BULK;
      this->_configurationDescriptor.outEndpoint.wMaxPacketSize=params.msc_bot_max_packet_size;         // LED reports are 1 byte

      // set up the IN endpoint descriptor

      this->_configurationDescriptor.inEndpoint.bEndpointAddress=IN_EP_ADDRESS;
      this->_configurationDescriptor.inEndpoint.bmAttributes=EndpointDescriptor::BULK;
      this->_configurationDescriptor.inEndpoint.wMaxPacketSize=params.msc_bot_max_packet_size;

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
    __attribute__((noinline)) inline void BotMscDevice<TPhy,Features...>::onEvent(UsbEventDescriptor& event) {
      // check for handled events

      switch(event.eventType) {

        case UsbEventDescriptor::EventType::CLASS_INIT:
          onInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_DEINIT:
          onDeInit();
          break;

        case UsbEventDescriptor::EventType::CLASS_SETUP:
          onSetup(static_cast<DeviceClassSdkSetupEvent&>(event));
          break;

        case UsbEventDescriptor::EventType::CLASS_DATA_IN:
          onDataIn();
          break;

        case UsbEventDescriptor::EventType::CLASS_DATA_OUT:
          onDataOut();
          break;

        default:    // warning supression
          break;
      }
    }


    /**
     * Initialise event
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onInit() {

      // set members

      _state=MscBotState::IDLE;
      _status=MscBotStatus::NORMAL;

      // reset SCSI state

      _scsi.onInit();

      // open the two endpoints

      USBD_LL_OpenEP(
          &this->_deviceHandle,
          IN_EP_ADDRESS,
          EndpointDescriptor::BULK,
          this->_configurationDescriptor.inEndpoint.wMaxPacketSize);

      USBD_LL_OpenEP(
          &this->_deviceHandle,
          OUT_EP_ADDRESS,
          EndpointDescriptor::BULK,
          this->_configurationDescriptor.outEndpoint.wMaxPacketSize);

      // flush the endpoints

      USBD_LL_FlushEP(&this->_deviceHandle,IN_EP_ADDRESS);
      USBD_LL_FlushEP(&this->_deviceHandle,OUT_EP_ADDRESS);

      // prepare EP to receive first BOT command

      static_assert(sizeof(_cbw)==32,"Compiler error: sizeof(MscBotCommandBlockWrapper)!=32");

      USBD_LL_PrepareReceive(
          &this->_deviceHandle,
          OUT_EP_ADDRESS,
          reinterpret_cast<uint8_t *>(&_cbw),
          MscBotCommandBlockWrapper::RECEIVE_SIZE);
    }


    /**
     * DeInitialise event
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onDeInit() {

      // close the two endpoints

      USBD_LL_CloseEP(&this->_deviceHandle,IN_EP_ADDRESS);
      USBD_LL_CloseEP(&this->_deviceHandle,OUT_EP_ADDRESS);

      // state is now idle

      _state=MscBotState::IDLE;
    }


    /**
     * Setup event
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onSetup(DeviceClassSdkSetupEvent& event) {

      switch (event.request.bmRequest & USB_REQ_TYPE_MASK) {

        case USB_REQ_TYPE_CLASS :
          onSetupClass(event);
          break;

        case USB_REQ_TYPE_STANDARD:
          onSetupInterfaceAndEndpoint(event);
          break;
      }
    }


    /**
     * Device class setup event
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onSetupClass(DeviceClassSdkSetupEvent& event) {

      switch(static_cast<MscBotClassRequest>(event.request.bRequest)) {

        case MscBotClassRequest::GET_MAX_LUN:
          onGetMaxLun(event);
          break;

        case MscBotClassRequest::RESET:
          onBotReset(event);
          break;

        default:
          USBD_CtlError(&this->_deviceHandle,&event.request);
          break;
      }
    }


    /**
     * Get the max logical unit number (LUN)
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onGetMaxLun(DeviceClassSdkSetupEvent& event) {

      if(event.request.wValue==0 && event.request.wLength==1 && ((event.request.bmRequest & 0x80)==0x80)) {

        MscBotGetMaxLunEvent event;

        // send the event that the user may respond to

        this->UsbEventSender.raiseEvent(event);

        // copy out the response

        _maxLun=event.maxLun;
        USBD_CtlSendData(&this->_deviceHandle,&_maxLun,1);
      }
      else
        USBD_CtlError(&this->_deviceHandle,&event.request);
    }


    /**
     * Get the max logical unit number (LUN)
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onBotReset(DeviceClassSdkSetupEvent& event) {

      if(event.request.wValue==0 && event.request.wLength==0 && ((event.request.bmRequest & 0x80)!=0x80)) {

        // set members

        _state=MscBotState::IDLE;
        _status=MscBotStatus::RECOVERY;

        // notify the event

        this->UsbEventSender.raiseEvent(MscBotResetEvent());
      }
      else
        USBD_CtlError(&this->_deviceHandle,&event.request);
    }


    /**
     * Setup interface and endpoint event
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onSetupInterfaceAndEndpoint(DeviceClassSdkSetupEvent& event) {

      switch(event.request.bRequest) {

        case USB_REQ_GET_INTERFACE:
          USBD_CtlSendData(&this->_deviceHandle,&_interface,1);
          break;

        case USB_REQ_SET_INTERFACE :
          _interface=event.request.wValue;
          break;

        case USB_REQ_CLEAR_FEATURE:
          onClearFeature(event);
          break;
      }
    }


    /**
     * Clear feature request
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onClearFeature(DeviceClassSdkSetupEvent& event) {

      // flush and close endpoint

      USBD_LL_FlushEP(&this->_deviceHandle,event.request.wIndex);     // flush the FIFO and clear the stall status
      USBD_LL_CloseEP(&this->_deviceHandle,event.request.wIndex);     // re-activate the EP

      if((event.request.wIndex & 0x80)==0x80) {
        USBD_LL_OpenEP(
                &this->_deviceHandle,
                IN_EP_ADDRESS,
                EndpointDescriptor::BULK,
                this->_configurationDescriptor.inEndpoint.wMaxPacketSize);
      }
      else {
        USBD_LL_OpenEP(
            &this->_deviceHandle,
            OUT_EP_ADDRESS,
            EndpointDescriptor::BULK,
            this->_configurationDescriptor.outEndpoint.wMaxPacketSize);
      }

      if(_status==MscBotStatus::ERROR) {

        // bad CBW signature

        USBD_LL_StallEP(&this->_deviceHandle,IN_EP_ADDRESS);
        _status=MscBotStatus::NORMAL;
      }
      else if(((event.request.wIndex & 0x80)==0x80) && _status!=MscBotStatus::RECOVERY)
        _csw.send<IN_EP_ADDRESS,OUT_EP_ADDRESS>(MscBotCswStatus::CMD_FAILED,_state,this->_deviceHandle,_cbw);
    }


    /**
     * Data in event
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onDataIn() {

      switch(_state) {

        case MscBotState::DATA_IN:
          if(!_scsi.processCmd(_cbw.bLUN,_cbw.CB,_cbw,_csw))
            _csw.send<IN_EP_ADDRESS,OUT_EP_ADDRESS>(MscBotCswStatus::CMD_FAILED,_state,this->_deviceHandle,_cbw);
          break;

        case MscBotState::SEND_DATA:
        case MscBotState::LAST_DATA_IN:
          _csw.send<IN_EP_ADDRESS,OUT_EP_ADDRESS>(MscBotCswStatus::CMD_PASSED,_state,this->_deviceHandle,_cbw);
          break;

        default:
          break;
      }
    }


    /**
     * Proccess MSC OUT data
     * @param epnum: endpoint index
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onDataOut() {

      switch(_state) {

        case MscBotState::IDLE:
          decodeCbw();
          break;

        case MscBotState::DATA_OUT:
          if(!_scsi.processCmd(_cbw.bLUN,_cbw.CB,_cbw,_csw))
            _csw.send<IN_EP_ADDRESS,OUT_EP_ADDRESS>(MscBotCswStatus::CMD_FAILED,_state,this->_deviceHandle,_cbw);
          break;

        default:      // warning supression
          break;
      }
    }


    /**
     * Decode the CBW command and set the BOT state machine
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::decodeCbw() {

      // update the CSW for the response

      _csw.dTag=_cbw.dTag;
      _csw.dDataResidue=_cbw.dDataLength;

      if(USBD_LL_GetRxDataSize(&this->_deviceHandle,OUT_EP_ADDRESS)!=MscBotCommandBlockWrapper::RECEIVE_SIZE ||
         _cbw.dSignature!=MscBotCommandBlockWrapper::SIGNATURE ||
         _cbw.bLUN>1 ||
         _cbw.bCBLength<1 ||
         _cbw.bCBLength>16) {

        _scsi.senseCode(_cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
        _status=MscBotStatus::ERROR;

        abortTransfer();

      } else {

        if(!_scsi.processCmd(_cbw.bLUN,_cbw.CB,_cbw,_csw)) {

          if(_state==MscBotState::NO_DATA)
            _csw.send<IN_EP_ADDRESS,OUT_EP_ADDRESS>(MscBotCswStatus::CMD_FAILED,_state,this->_deviceHandle,_cbw);
          else
            abortTransfer();
        }
        else if(_state!=MscBotState::DATA_IN && _state!=MscBotState::DATA_OUT && _state!=MscBotState::LAST_DATA_IN) {

          // burst xfer handled internally

          if(_scsi.getDataSize())
            sendData(_scsi.getData(),_scsi.getDataSize());
          else
            _csw.send<IN_EP_ADDRESS,OUT_EP_ADDRESS>(MscBotCswStatus::CMD_PASSED,_state,this->_deviceHandle,_cbw);
        }
      }
    }


    /**
     * @brief  Abort the current transfer
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::abortTransfer() {

      // stall endpoints

      if(_cbw.bmFlags==0 && _cbw.dDataLength!=0 && _status==MscBotStatus::NORMAL)
        USBD_LL_StallEP(&this->_deviceHandle,OUT_EP_ADDRESS);

      USBD_LL_StallEP(&this->_deviceHandle,IN_EP_ADDRESS);

      // prepare to receive next command

      if(_status==MscBotStatus::ERROR)
        USBD_LL_PrepareReceive(
            &this->_deviceHandle,
            OUT_EP_ADDRESS,
            reinterpret_cast<uint8_t *>(&_cbw),
            MscBotCommandBlockWrapper::RECEIVE_SIZE);
    }


    /**
     * Send the requested data
     * @param buf pointer to data buffer
     * @param len Data Length
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::sendData(uint8_t *buf,uint16_t len) {

      // validate length

      if(len>_cbw.dDataLength)
        len=_cbw.dDataLength;

      _csw.dDataResidue-=len;
      _csw.bStatus=MscBotCswStatus::CMD_PASSED;
      _state=MscBotState::SEND_DATA;

      USBD_LL_Transmit(&this->_deviceHandle,IN_EP_ADDRESS,buf,len);
    }
  }
}
