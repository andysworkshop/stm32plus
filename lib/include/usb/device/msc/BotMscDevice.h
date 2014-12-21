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
      EndpointDescriptor outEndpoint;
      EndpointDescriptor inEndpoint;
    } __attribute__((packed));


    /**
     * Derivation of MscDevice to provide support for the Bulk Only Transport (BOT)
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

         /**
          * Customisable parameters for this MSC BOT device
          */

         struct Parameters : MscDeviceBase::Parameters {

           uint16_t msc_bot_max_packet_size;        // default is 64 bytes
           uint16_t msc_bot_media_packet_size;      // default is 8192 bytes

           Parameters() {
             msc_bot_max_packet_size=0x40;
             msc_bot_media_packet_size=8192;
           }
         };

       protected:

         /*
          * Endpoint addresses
          */

         enum {
           IN_EP_ADDRESS = EndpointDescriptor::IN | 1,     // IN endpoint address
           OUT_EP_ADDRESS = EndpointDescriptor::OUT | 2,   // OUT endpoint address
         };

         /*
          * Response packet sizes
          */

         enum {
           READ_FORMAT_CAPACITY_DATA_LEN = 0x0C,
           READ_CAPACITY10_DATA_LEN      = 0x08,
           MODE_SENSE10_DATA_LEN         = 0x08,
           MODE_SENSE6_DATA_LEN          = 0x04,
           REQUEST_SENSE_DATA_LEN        = 0x12,
           STANDARD_INQUIRY_DATA_LEN     = 0x24
         };

         uint8_t _interface;
         MscBotState _state;
         MscBotStatus _status;
         MscBotCommandBlockWrapper _cbw;
         MscBotCommandStatusWrapper _csw;
         MscScsi _scsi;
         uint16_t _packetLength;
         scoped_array<uint8_t> _packetData;

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
        void sendCsw(MscBotCswStatus status);
        void onDataIn();

        bool processScsiCmd(uint8_t lun,uint8_t *params);
        bool scsiTestUnitReady(uint8_t lun);
        bool scsiRequestSense(uint8_t *params);
        bool scsiInquiry(uint8_t lun,uint8_t *params);
        bool scsiStartStopUnit(uint8_t lun,uint8_t *params);
        bool scsiModeSense6(uint8_t lun,uint8_t *params);
        bool scsiModeSense10(uint8_t lun,uint8_t *params);
        bool scsiReadFormatCapacity(uint8_t lun,uint8_t *params);
        bool scsiReadCapacity10(uint8_t lun,uint8_t *params);
        bool scsiRead10(uint8_t lun,uint8_t *params);
        bool scsiWrite10(uint8_t lun,uint8_t *params);
        bool scsiVerify10(uint8_t lun,uint8_t *params);
        void scsiSenseCode(uint8_t lun,uint8_t sKey,uint8_t ASC);

      public:
        BotMscDevice();
        ~BotMscDevice();

        bool initialise(Parameters& params);
    };


    /**
     * Constructor
     */

    template<class TPhy,template <class> class... Features>
    inline BotMscDevice<TPhy,Features...>::BotMscDevice() {

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

      if(!MscDeviceBase::initialise(params))
        return false;

      // create the media packet

      _packetData.reset(new uint8_t[params.msc_bot_media_packet_size]);

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
      _scsi.senseHead=_scsi.senseTail=0;

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

      static_assert(sizeof(_cbw)==31,"Compiler error: sizeof(MscBotCommandBlockWrapper)!=31");
      USBD_LL_PrepareReceive(&this->_deviceHandle,OUT_EP_ADDRESS,reinterpret_cast<uint8_t *>(&_cbw),sizeof(_cbw));
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
        USBD_CtlSendData(&this->_deviceHandle,&event.maxLun,1);
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
        sendCsw(MscBotCswStatus::CMD_FAILED);
    }


    /**
     * Send the command status wrapper
     * @param status The status to send
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::sendCsw(MscBotCswStatus status) {

      _csw.bStatus=status;
      _state=MscBotState::IDLE;

      // send the status

      USBD_LL_Transmit(&this->_deviceHandle,IN_EP_ADDRESS,reinterpret_cast<uint8_t *>(&_csw),sizeof(_csw));

      // prepare EP to receive next command

      USBD_LL_PrepareReceive(&this->_deviceHandle,OUT_EP_ADDRESS,reinterpret_cast<uint8_t *>(&_cbw),sizeof(_cbw));
    }


    /**
     * Data in event
     * @param event The event details
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::onDataIn() {

      switch(_state) {

        case MscBotState::DATA_IN:
          if(!processScsiCmd(_cbw.bLUN,&_cbw.CB[0]))
            sendCsw(MscBotCswStatus::CMD_FAILED);
          break;

        case MscBotState::SEND_DATA:
        case MscBotState::LAST_DATA_IN:
          sendCsw(MscBotCswStatus::CMD_PASSED);
          break;

        default:
          break;
      }
    }


    /**
     * Process the SCSI command
     * @param lun logical unit number
     * @param params command and parameters
     * @return true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool BotMscDevice<TPhy,Features...>::processScsiCmd(uint8_t lun,uint8_t *params) {

      switch(static_cast<MscScsiCommand>(params[0])) {

        case MscScsiCommand::TEST_UNIT_READY:
          return scsiTestUnitReady(lun);

        case MscScsiCommand::REQUEST_SENSE:
          return scsiRequestSense(params);

        case MscScsiCommand::INQUIRY:
          return scsiInquiry(lun,params);

        case MscScsiCommand::START_STOP_UNIT:
          return scsiStartStopUnit(lun,params);

        case MscScsiCommand::ALLOW_MEDIUM_REMOVAL:
          return scsiStartStopUnit(lun,params);

        case MscScsiCommand::MODE_SENSE6:
          return scsiModeSense6(lun,params);

        case MscScsiCommand::MODE_SENSE10:
          return scsiModeSense10(lun,params);

        case MscScsiCommand::READ_FORMAT_CAPACITIES:
          return scsiReadFormatCapacity(lun,params);

        case MscScsiCommand::READ_CAPACITY10:
          return scsiReadCapacity10(lun,params);

        case MscScsiCommand::READ10:
          return scsiRead10(lun,params);

        case MscScsiCommand::WRITE10:
          return scsiWrite10(lun,params);

        case MscScsiCommand::VERIFY10:
          return scsiVerify10(lun,params);

        default:
          scsiSenseCode(lun,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
          return false;
      }
    }


    /**
     * Process SCSI Test Unit Ready Command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true if it worked
     */

    template<class TPhy,template <class> class... Features>
    inline bool BotMscDevice<TPhy,Features...>::scsiTestUnitReady(uint8_t lun) {

      // case 9 : Hi > D0

      if(_cbw.dDataLength) {

        scsiSenseCode(_cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
        return false;
      }

      // send the 'is ready' event

      MscBotIsReadyEvent event(lun);
      this->UsbEventSender.raiseEvent(event);

      if(event.isReady) {
        _packetLength=0;
        return true;
      }
      else {

        scsiSenseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::MEDIUM_NOT_PRESENT);

        _state=MscBotState::NO_DATA;
        return false;
      }
    }


    /**
     * @brief  Process Request Sense command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true if it worked
     */

    template<class TPhy,template<class > class... Features>
    inline bool BotMscDevice<TPhy,Features...>::scsiRequestSense(uint8_t *params) {

      uint8_t *ptr;

      // clear out the data

      ptr=_packetData.get();
      memset(ptr,'\0',REQUEST_SENSE_DATA_LEN);

      // create the packet

      ptr[0]=0x70;
      ptr[7]=REQUEST_SENSE_DATA_LEN-6;

      if(_scsi.senseHead!=_scsi.senseTail) {

        ptr[2]=_scsi.sense[_scsi.senseHead].Skey;
        ptr[12]=_scsi.sense[_scsi.senseHead].w.b.ASCQ;
        ptr[13]=_scsi.sense[_scsi.senseHead].w.b.ASC;

        _scsi.senseHead++;

        if(_scsi.senseHead==MscScsi::SENSE_LIST_DEPTH)
          _scsi.senseHead=0;
      }

      _packetLength=REQUEST_SENSE_DATA_LEN;

      if(params[4]<=REQUEST_SENSE_DATA_LEN)
        _packetLength=params[4];

      return true;
    }


    /**
     * @brief  Load the last error code in the error list
     * @param  lun: Logical unit number
     * @param  sKey: Sense Key
     * @param  ASC: Additional Sense Key
     * @retval none
     */

    template<class TPhy,template <class> class... Features>
    inline void BotMscDevice<TPhy,Features...>::scsiSenseCode(uint8_t /* lun */,uint8_t sKey,uint8_t ASC) {

      _scsi.sense[_scsi.senseTail].Skey=sKey;
      _scsi.sense[_scsi.senseTail].w.ASC=ASC << 8;

      _scsi.senseTail++;

      if(_scsi.senseTail==MscScsi::SENSE_LIST_DEPTH)
        _scsi.senseTail=0;
    }
  }
}
