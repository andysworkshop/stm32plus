/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace usb {


    /**
     * SCSI interaction class. Handles all the mechanics of processing the SCSI
     * commands that actually manipulate the disk and its data
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    class MscScsi {

      public:

        /**
         * Parameters structure
         */

        struct Parameters {

          uint16_t msc_media_packet_size;      // default is 8192 bytes

           Parameters() {
             msc_media_packet_size=8192;
           }
        };

      protected:

        enum {
          SENSE_LIST_DEPTH = 4
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

        MscScsiSense _sense[SENSE_LIST_DEPTH];
        uint8_t _senseHead;
        uint8_t _senseTail;

        uint16_t _blkSize;
        uint32_t _blkNbr;

        uint32_t _blkAddr;
        uint32_t _blkLen;

        uint16_t _packetSize;
        uint16_t _maxPacketSize;
        scoped_array<uint8_t> _packetData;

        MscBotState& _botState;
        UsbEventSource& _eventSource;
        USBD_HandleTypeDef& _deviceHandle;

      protected:
        bool processRead(uint8_t lun,MscBotCommandStatusWrapper& csw);
        bool processWrite(uint8_t lun,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw);
        bool checkAddressRange(uint8_t lun,uint32_t blk_offset,uint16_t blk_nbr);

      public:
        MscScsi(MscBotState& botState,UsbEventSource& eventSource,USBD_HandleTypeDef& deviceHandle);
        bool initialise(const Parameters& params);

        void onInit();

        bool processCmd(uint8_t lun,uint8_t *params,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw);
        bool testUnitReady(uint8_t lun,MscBotCommandBlockWrapper& cbw);
        bool requestSense(uint8_t *params);
        bool inquiry(uint8_t lun,uint8_t *params);
        bool startStopUnit();
        bool modeSense6(uint8_t lun);
        bool modeSense10();
        bool readFormatCapacity(uint8_t lun);
        bool readCapacity10(uint8_t lun);
        bool read10(uint8_t lun,uint8_t *params,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw);
        bool write10(uint8_t lun,uint8_t *params,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw);
        bool verify10(uint8_t lun,uint8_t *params);
        void senseCode(uint8_t lun,uint8_t sKey,uint8_t asc);

        uint16_t getDataSize() const;
        uint8_t *getData() const;
    };


    /**
     * Constructor
     * @param botState The BOT state machine state
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline MscScsi<TInEndpointAddress,TOutEndpointAddress>::MscScsi(MscBotState& botState,UsbEventSource& eventSource,USBD_HandleTypeDef& deviceHandle)
      : _botState(botState),
        _eventSource(eventSource),
        _deviceHandle(deviceHandle) {
    }


    /**
     * Initialise the class
     * @param params The parameters
     * @return true
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::initialise(const Parameters& params) {

      _maxPacketSize=params.msc_media_packet_size;
      _packetData.reset(new uint8_t[_maxPacketSize]);

      return true;
    }


    /**
     * Re-initialise the class
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline void MscScsi<TInEndpointAddress,TOutEndpointAddress>::onInit() {
      _senseHead=_senseTail=0;
    }


    /**
     * Process the SCSI command
     * @param lun logical unit number
     * @param params command and parameters
     * @return true if it worked
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::processCmd(uint8_t lun,uint8_t *params,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw) {

      switch(static_cast<MscScsiCommand>(params[0])) {

        case MscScsiCommand::TEST_UNIT_READY:
          return testUnitReady(lun,cbw);

        case MscScsiCommand::REQUEST_SENSE:
          return requestSense(params);

        case MscScsiCommand::INQUIRY:
          return inquiry(lun,params);

        case MscScsiCommand::START_STOP_UNIT:
          return startStopUnit();

        case MscScsiCommand::ALLOW_MEDIUM_REMOVAL:
          return startStopUnit();

        case MscScsiCommand::MODE_SENSE6:
          return modeSense6(lun);

        case MscScsiCommand::MODE_SENSE10:
          return modeSense10();

        case MscScsiCommand::READ_FORMAT_CAPACITIES:
          return readFormatCapacity(lun);

        case MscScsiCommand::READ_CAPACITY10:
          return readCapacity10(lun);

        case MscScsiCommand::READ10:
          return read10(lun,params,cbw,csw);

        case MscScsiCommand::WRITE10:
          return write10(lun,params,cbw,csw);

        case MscScsiCommand::VERIFY10:
          return verify10(lun,params);

        default:
          senseCode(lun,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
          return false;
      }
    }


    /**
     * Process SCSI Test Unit Ready Command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true if it worked
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::testUnitReady(uint8_t lun,MscBotCommandBlockWrapper& cbw) {

      // case 9 : Hi > D0

      if(cbw.dDataLength) {

        senseCode(cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
        return false;
      }

      // send the 'is ready' event

      MscBotIsReadyEvent event(lun);
      _eventSource.UsbEventSender.raiseEvent(event);

      if(event.isReady) {
        _packetSize=0;
        return true;
      }
      else {

        senseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::MEDIUM_NOT_PRESENT);

        _botState=MscBotState::NO_DATA;
        return false;
      }
    }


    /**
     * @brief  Process Request Sense command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true if it worked
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::requestSense(uint8_t *params) {

      uint8_t *ptr;

      // clear out the data

      ptr=_packetData.get();
      memset(ptr,'\0',REQUEST_SENSE_DATA_LEN);

      // create the packet

      ptr[0]=0x70;
      ptr[7]=REQUEST_SENSE_DATA_LEN-6;

      if(_senseHead!=_senseTail) {

        ptr[2]=_sense[_senseHead].Skey;
        ptr[12]=_sense[_senseHead].w.b.ASCQ;
        ptr[13]=_sense[_senseHead].w.b.ASC;

        _senseHead++;

        if(_senseHead==SENSE_LIST_DEPTH)
          _senseHead=0;
      }

      _packetSize=REQUEST_SENSE_DATA_LEN;

      if(params[4]<REQUEST_SENSE_DATA_LEN)
        _packetSize=params[4];

      return true;
    }


    /**
     * Process Inquiry command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true if it worked
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::inquiry(uint8_t lun,uint8_t *params) {

      static const uint8_t page00[7] = { 0,0,0,3,0,0x80,0x83 };

      if(params[1] & 0x01) {

        // Evpd is set

        memcpy(_packetData.get(),page00,sizeof(page00));
        _packetSize=7;
      }
      else {

        // send the event that gets the page data

        MscBotGetEnquiryPageEvent event(lun);
        _eventSource.UsbEventSender.raiseEvent(event);

        // packet size is in the page data

        _packetSize=event.enquiryPage[4]+5;

        // trim to limit and copy

        if(params[4]<_packetSize)
          _packetSize=params[4];

        memcpy(_packetData.get(),event.enquiryPage,_packetSize);
      }

      return true;
    }


    /**
     * Process Start Stop Unit command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::startStopUnit() {

      _packetSize=0;
      return true;
    }


    /**
     * Process Mode Sense6 command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::modeSense6(uint8_t lun) {

      uint8_t *ptr;

      // get the write-protection status

      MscBotIsWriteProtectedEvent writeProtectedEvent(lun);
      _eventSource.UsbEventSender.raiseEvent(writeProtectedEvent);

      ptr=_packetData.get();

      ptr[0]=3;     // bytes to follow
      ptr[1]=0;     // 00h for SBC devices
      ptr[2]=writeProtectedEvent.isWriteProtected ? 0x80 : 0;
      ptr[3]=0;     // no block descriptors

      _packetSize=4;
      return true;
    }


    /**
     * @brief  Process Mode Sense10 command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval status
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::modeSense10() {

      static const uint8_t data[8]={ 0,6,0,0,0,0,0,0 };

      memcpy(_packetData.get(),data,sizeof(data));
      _packetSize=sizeof(data);
      return true;
    }


    /**
     * Process Read Format Capacity command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true/false
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::readFormatCapacity(uint8_t lun) {

      MscBotGetCapacityEvent event(lun);
      uint8_t *ptr;

      _eventSource.UsbEventSender.raiseEvent(event);

      if(event.ready) {

        ptr=_packetData.get();

        _blkSize=event.blockSize;
        _blkNbr=event.blockCount;

        ptr[0]=0;
        ptr[1]=0;
        ptr[2]=0;

        ptr[3]=16;                  // 2 descriptors

        ptr[4]=ptr[12]=_blkNbr >> 24;       // number of blocks
        ptr[5]=ptr[13]=_blkNbr >> 16;
        ptr[6]=ptr[14]=_blkNbr >> 8;
        ptr[7]=ptr[15]=_blkNbr;

        ptr[8]=2;                   // formatted media
        ptr[9]=ptr[17]=_blkSize >> 16;
        ptr[10]=ptr[18]=_blkSize >> 8;
        ptr[11]=ptr[19]=_blkSize;

        ptr[16]=0;                  // addressable blocks

        _packetSize=20;
        return true;
      }
      else {
        senseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::MEDIUM_NOT_PRESENT);
        return false;
      }
    }


    /**
     * Process Read Capacity 10 command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true/false
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::readCapacity10(uint8_t lun) {

      MscBotGetCapacityEvent event(lun);
      uint8_t *ptr;

      _eventSource.UsbEventSender.raiseEvent(event);

      if(event.ready) {

        _blkSize=event.blockSize;
        _blkNbr=event.blockCount;

        ptr=_packetData.get();

        ptr[0]=(_blkNbr-1) >> 24;
        ptr[1]=(_blkNbr-1) >> 16;
        ptr[2]=(_blkNbr-1) >> 8;
        ptr[3]=_blkNbr-1;

        ptr[4]=_blkSize >> 24;
        ptr[5]=_blkSize >> 16;
        ptr[6]=_blkSize >> 8;
        ptr[7]=_blkSize;

        _packetSize=8;
        return true;
      }
      else {
        senseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::MEDIUM_NOT_PRESENT);
        return false;
      }
    }


    /**
     * @brief  SCSI_Read10
     *         Process Read10 command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval status
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::read10(uint8_t lun,uint8_t *params,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw) {

      if(_botState==MscBotState::IDLE) {

        // case 10 : Ho <> Di

        if((cbw.bmFlags & 0x80)!=0x80) {

          senseCode(cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
          return false;
        }

        // send the 'is ready' event

        MscBotIsReadyEvent event(lun);
        _eventSource.UsbEventSender.raiseEvent(event);

        if(!event.isReady) {

          senseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::MEDIUM_NOT_PRESENT);
          return false;
        }

        // get block address and length

        _blkAddr=(params[2] << 24) | (params[3] << 16) | (params[4] << 8) | params[5];
        _blkLen=(params[7] << 8) | params[8];

        if(!checkAddressRange(lun,_blkAddr,_blkLen))
          return false;

        // set the data-in state

        _botState=MscBotState::DATA_IN;

        // convert to byte addresses

        _blkAddr*=_blkSize;
        _blkLen*=_blkSize;

        // cases 4,5 : Hi <> Dn

        if(cbw.dDataLength!=_blkLen) {
          senseCode(cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
          return false;
        }
      }

      // do the read operation

      _packetSize=_maxPacketSize;
      return processRead(lun,csw);
    }


    /**
     * Handle Read Process
     * @param lun: Logical unit number
     * @retval true if it worked
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::processRead(uint8_t lun,MscBotCommandStatusWrapper& csw) {

      uint32_t len;

      // send the event to read the data

      len=_blkLen<_maxPacketSize ? _blkLen : _maxPacketSize;

      MscBotReadEvent event(lun,_packetData.get(),_blkAddr/_blkSize,len/_blkSize);

      _eventSource.UsbEventSender.raiseEvent(event);

      if(!event.success) {
        senseCode(lun,MscScsiSense::HARDWARE_ERROR,MscScsiSense::UNRECOVERED_READ_ERROR);
        return false;
      }

      // transmit to the host

      USBD_LL_Transmit(&_deviceHandle,TInEndpointAddress,_packetData.get(),len);

      // update addresses for sequential read

      _blkAddr+=len;
      _blkLen-=len;

      // case 6 : Hi = Di

      csw.dDataResidue-=len;

      if(_blkLen==0)
        _botState=MscBotState::LAST_DATA_IN;

      return true;
    }


    /**
     * Process Write10 command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true/false
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::write10(uint8_t lun ,uint8_t *params,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw) {

      if(_botState==MscBotState::IDLE) {

        // case 8 : Hi <> Do

        if((cbw.bmFlags & 0x80)==0x80) {
          senseCode(cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
          return false;
        }

        // send the 'is ready' event

        MscBotIsReadyEvent readyEvent(lun);
        _eventSource.UsbEventSender.raiseEvent(readyEvent);

        if(!readyEvent.isReady) {
          senseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::MEDIUM_NOT_PRESENT);
          return false;
        }

        // send the 'is write protected' event

        MscBotIsWriteProtectedEvent writeProtectedEvent(lun);
        _eventSource.UsbEventSender.raiseEvent(writeProtectedEvent);

        if(writeProtectedEvent.isWriteProtected) {
          senseCode(lun,MscScsiSense::NOT_READY,MscScsiSense::WRITE_PROTECTED);
          return false;
        }

        // get address and size

        _blkAddr=(params[2] << 24) | (params[3] << 16) | (params[4] << 8) | params[5];
        _blkLen=(params[7] << 8) | params[8];

        // check if LBA address is in the right range

        if(!checkAddressRange(lun,_blkAddr,_blkLen))
          return false;

        // convert to byte addresses

        _blkAddr*=_blkSize;
        _blkLen*=_blkSize;

        // cases 3,11,13 : Hn,Ho <> D0

        if(cbw.dDataLength!=_blkLen) {
          senseCode(cbw.bLUN,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_CDB);
          return false;
        }

        // Prepare EP to receive first data packet

        _botState=MscBotState::DATA_OUT;

        USBD_LL_PrepareReceive(
            &_deviceHandle,
            TOutEndpointAddress,
            _packetData.get(),
            _blkLen<_maxPacketSize ? _blkLen : _maxPacketSize);

        return true;
      }
      else  // write process ongoing
        return processWrite(lun,cbw,csw);
    }


    /**
     * Process ongoing write
     * @param lun The logical unit number
     * @return true/false
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::processWrite(uint8_t lun,MscBotCommandBlockWrapper& cbw,MscBotCommandStatusWrapper& csw) {

      uint32_t len;

      // send the write event

      len=_blkLen<_maxPacketSize ? _blkLen : _maxPacketSize;

      MscBotWriteEvent event(lun,_packetData.get(),_blkAddr/_blkSize,len/_blkSize);

      _eventSource.UsbEventSender.raiseEvent(event);

      if(!event.success) {
        senseCode(lun,MscScsiSense::HARDWARE_ERROR,MscScsiSense::WRITE_FAULT);
        return false;
      }

      // update for sequential write

      _blkAddr+=len;
      _blkLen-=len;

      // case 12 : Ho = Do

      csw.dDataResidue-=len;

      if(_blkLen==0)
        csw.send<TInEndpointAddress,TOutEndpointAddress>(MscBotCswStatus::CMD_PASSED,_botState,_deviceHandle,cbw);
      else {

        // prepare to receive the next packet

        USBD_LL_PrepareReceive(
            &_deviceHandle,
            TOutEndpointAddress,
            _packetData.get(),
            _blkLen<_maxPacketSize ? _blkLen : _maxPacketSize);
      }

      return true;
    }


    /**
     * Process Verify10 command
     * @param  lun: Logical unit number
     * @param  params: Command parameters
     * @retval true/false
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::verify10(uint8_t lun,uint8_t *params) {

      if((params[1] & 0x02)==0x02) {
        senseCode(lun,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::INVALID_FIELED_IN_COMMAND);
        return false;
      }

      if(!checkAddressRange(lun,_blkAddr,_blkLen))
        return false;

      _packetSize=0;
      return true;
    }


    /**
     * Check address range
     * @param  lun: Logical unit number
     * @param  blk_offset: first block address
     * @param  blk_nbr: number of block to be processed
     * @retval true if it worked
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline bool MscScsi<TInEndpointAddress,TOutEndpointAddress>::checkAddressRange(uint8_t lun,uint32_t blk_offset,uint16_t blk_nbr) {

      if(blk_offset+blk_nbr>_blkNbr) {
        senseCode(lun,MscScsiSense::ILLEGAL_REQUEST,MscScsiSense::ADDRESS_OUT_OF_RANGE);
        return false;
      }

      return true;
    }


    /**
     * @brief  Load the last error code in the error list
     * @param  lun: Logical unit number
     * @param  sKey: Sense Key
     * @param  ASC: Additional Sense Key
     * @retval none
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline void MscScsi<TInEndpointAddress,TOutEndpointAddress>::senseCode(uint8_t /* lun */,uint8_t sKey,uint8_t asc) {

      _sense[_senseTail].Skey=sKey;
      _sense[_senseTail].w.ASC=asc << 8;

      _senseTail++;

      if(_senseTail==MscScsi::SENSE_LIST_DEPTH)
        _senseTail=0;
    }


    /**
     * Get the data packet size
     * @return The data packet size
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline uint16_t MscScsi<TInEndpointAddress,TOutEndpointAddress>::getDataSize() const {
      return _packetSize;
    }


    /**
     * Get the data packet size
     * @return The data packet size
     */

    template<uint8_t TInEndpointAddress,uint8_t TOutEndpointAddress>
    inline uint8_t *MscScsi<TInEndpointAddress,TOutEndpointAddress>::getData() const {
      return _packetData.get();
    }
  }
}
