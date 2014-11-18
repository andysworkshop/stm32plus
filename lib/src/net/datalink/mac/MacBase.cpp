/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net.h"


// Global pointer for last received frame infos

extern "C" ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;


namespace stm32plus {
  namespace net {


    /**
     * Startup the class
     * @return true if it worked
     */

    bool MacBase::startup() {

      // announce our MAC address to everyone

      this->NetworkNotificationEventSender.raiseEvent(MacAddressAnnouncementEvent(_params.mac_address));
      return true;
    }


    /**
     * Initialise
     * @param params parameters structure
     * @return true
     */

    bool MacBase::initialise(const Parameters& params) {

      ETH_DMADESCTypeDef *txdesc;
      int i;

      // save parameters

      _params=params;

      // subscribe to send and notify events

      this->NetworkSendEventSender.insertSubscriber(NetworkSendEventSourceSlot::bind(this,&MacBase::onSend));

      // set our MAC address

      ETH_MACAddressConfig(ETH_MAC_Address0,const_cast<uint8_t *>(params.mac_address.macAddress));

      // initialise the receive descriptor chain

      _receiveDmaDescriptors.reset(new ETH_DMADESCTypeDef[params.mac_receiveBufferCount]);
      _receiveBuffers.reset(new uint8_t[params.mac_receiveBufferCount][ETH_MAX_PACKET_SIZE]);
      ETH_DMARxDescChainInit(&_receiveDmaDescriptors[0],&_receiveBuffers[0][0],params.mac_receiveBufferCount);

      // enable interrupts on all the receive buffers

      for(i=0;i<params.mac_receiveBufferCount;i++)
        ETH_DMARxDescReceiveITConfig(&_receiveDmaDescriptors[i],ENABLE);

      // initialise the transmit descriptor ring

      _transmitDmaDescriptors.reset(new ETH_DMADESCTypeDef[params.mac_transmitBufferCount]);
      _transmitNetBuffers.reset(new NetBuffer *[params.mac_transmitBufferCount]);
      txdesc=_transmitDmaDescriptors.get();

      // initialise the transmit ring buffer. the DMA descriptors are in contiguous memory blocks
      // and we make full use of the ability to transmit up to 2 blocks per descriptor

      for(i=0;i<params.mac_transmitBufferCount;i++) {

        txdesc->Status=ETH_DMATxDesc_IC;        // interrupt on complete
        txdesc->Buffer1Addr=0;                  // not yet, nothing to send
        txdesc->Buffer2NextDescAddr=0;          // not yet, nothing to send

        if(i==params.mac_transmitBufferCount-1)
          txdesc->Status|=ETH_DMATxDesc_TER;    // end of ring

        // clear out the NetBuffer pointer associated with this entry

        _transmitNetBuffers[i]=nullptr;

        // advance to next buffer

        txdesc++;
      }

      // set the base table address

      ETH->DMATDLAR=reinterpret_cast<uint32_t>(_transmitDmaDescriptors.get());

      // this is the next one to consider sending

      _transmitBufferIndex=0;
      return true;
    }


    /**
     * Handle the receive DMA interrupt. Set up an EthernetFrame structure and notify observers. If an
     * error occurred, notify of the error
     */

    void MacBase::handleReceiveInterrupt() {

      FrameTypeDef frame;

      // loop over received frames

      for(frame=ETH_Get_Received_Frame_interrupt(_params.mac_receiveBufferCount);frame.buffer;frame=ETH_Get_Received_Frame_interrupt(_params.mac_receiveBufferCount))
        processReceivedFrame(frame);
    }


    /**
     * Fully process a received frame
     * @param frame The frame definition to process
     */

    void MacBase::processReceivedFrame(const FrameTypeDef& frame) {

      volatile ETH_DMADESCTypeDef *DMARxNextDesc;
      uint32_t i,context;
      EthernetFrame ef;

      // check for errors (_ES is the OR of all error flags into one bit)

      if((frame.descriptor->Status & ETH_DMARxDesc_ES)!=0) {

        // an error has occurred

        switch(frame.descriptor->Status & (ETH_DMARxDesc_CE | ETH_DMARxDesc_RE | ETH_DMARxDesc_RWT | ETH_DMARxDesc_LC | ETH_DMARxDesc_IPV4HCE | ETH_DMARxDesc_OE | ETH_DMARxDesc_DE)) {

          case ETH_DMARxDesc_CE:
            context=E_CRC;
            break;

          case ETH_DMARxDesc_RE:
            context=E_RECEIVE;
            break;

          case ETH_DMARxDesc_RWT:
            context=E_WATCHDOG;
            break;

          case ETH_DMARxDesc_LC:
            context=E_LATE_COLLISION;
            break;

          case ETH_DMARxDesc_IPV4HCE:
            context=E_IP_HEADER_CHECKSUM;
            break;

          case ETH_DMARxDesc_OE:
            context=E_OVERFLOW;
            break;

          case ETH_DMARxDesc_DE:
            context=E_TRUNCATED;
            break;

          default:
            context=E_UNSPECIFIED;
            break;
        }

        // notify the observers of the error

        this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,context);
      }
      else {

#if defined(USE_ENHANCED_DMA_DESCRIPTORS)

        if((frame.descriptor->Status & (ETH_DMARxDesc_LS | ETH_DMARxDesc_MAMPCE))==(ETH_DMARxDesc_LS | ETH_DMARxDesc_MAMPCE)) {

          // extended info is available, check for errors in RDES4

          if((frame.descriptor->ExtendedStatus & (ETH_DMAPTPRxDesc_IPPE | ETH_DMAPTPRxDesc_IPHE))!=0) {

            // set the error code

            if((frame.descriptor->ExtendedStatus & ETH_DMAPTPRxDesc_IPPE)!=0)
              context=E_PAYLOAD;
            else
              context=E_HEADER;

            // notify observers of the error (PE = payload, HE= header)

            this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,context);
          }
          else {

            // set the various frame detection flags

            if((frame.descriptor->ExtendedStatus & ETH_DMAPTPRxDesc_IPV4PR)!=0)
              ef.flags|=EthernetFrame::FLAG_IP;
            else if((frame.descriptor->ExtendedStatus & ETH_DMAPTPRxDesc_IPV6PR)!=0)
              ef.flags|=EthernetFrame::FLAG_IP6;

            if((frame.descriptor->ExtendedStatus & ETH_DMAPTPRxDesc_IPPT_UDP)!=0)
              ef.flags|=EthernetFrame::FLAG_UDP;
            else if((frame.descriptor->ExtendedStatus & ETH_DMAPTPRxDesc_IPPT_TCP)!=0)
              ef.flags|=EthernetFrame::FLAG_TCP;
            else if((frame.descriptor->ExtendedStatus & ETH_DMAPTPRxDesc_IPPT_ICMP)!=0)
              ef.flags|=EthernetFrame::FLAG_ICMP;

            // extended frame with possible flags received

            if(setupEthernetFrame(frame,ef))
              NetworkReceiveEventSender.raiseEvent(DatalinkFrameEvent(ef));
          }
        }
        else {

          // basic frame with no extended info received OK

          if(setupEthernetFrame(frame,ef))
            NetworkReceiveEventSender.raiseEvent(DatalinkFrameEvent(ef));
        }
#else
        // basic frame with no extended info received OK

        if(setupEthernetFrame(frame,ef))
          NetworkReceiveEventSender.raiseEvent(DatalinkFrameEvent(ef));
#endif
      }

      // release descriptors to DMA
      // check if received frame with multiple DMA buffer segments

      if(DMA_RX_FRAME_infos->Seg_Count>1)
        DMARxNextDesc=DMA_RX_FRAME_infos->FS_Rx_Desc;
      else
        DMARxNextDesc=frame.descriptor;

      // set Own bit in Rx descriptors: gives the buffers back to DMA

      for(i=0;i<DMA_RX_FRAME_infos->Seg_Count;i++) {
        DMARxNextDesc->Status=ETH_DMARxDesc_OWN;
        DMARxNextDesc=(ETH_DMADESCTypeDef *)(DMARxNextDesc->Buffer2NextDescAddr);
      }

      // clear Segment_Count

      DMA_RX_FRAME_infos->Seg_Count=0;

      // When Rx Buffer unavailable flag is set: clear it and resume reception

      if((ETH->DMASR & ETH_DMASR_RBUS)!=(uint32_t)RESET) {

        // clear RBUS ETHERNET DMA flag

        ETH->DMASR=ETH_DMASR_RBUS;

        // Resume DMA reception

        ETH->DMARPDR=0;
      }
    }


    /**
     * Set up the EthernetFrame structure
     * @param fd The incoming frame definition
     * @param ef The extended EthernetFrame structure to fill in
     * @param true if it worked.
     */

    bool MacBase::setupEthernetFrame(const FrameTypeDef& fd,EthernetFrame& ef) const {

      EthernetFrameData *efd;

      efd=reinterpret_cast<EthernetFrameData *>(fd.buffer);

      // source and dest are mandatory

      ef.destinationMac=&efd->eth_destinationAddress;
      ef.sourceMac=&efd->eth_sourceAddress;
      ef.frameSource=DatalinkFrame::FrameSource::ETHERNET_FRAME;

      // qtag is optional and supported

      if(efd->eth_etherType==0x0081) {        // big-endian comparison for 0x8100

        EthernetTaggedFrameData *eftd;

        // get the protocol, which might be a length if 802.3 not v2.

        eftd=reinterpret_cast<EthernetTaggedFrameData *>(fd.buffer);
        ef.protocol=NetUtil::ntohs(eftd->eth_etherType);

        if(ef.protocol<0x0600) {

          EthernetTaggedSnapFrameData *etsfd;

          // this is an 802.3 frame with a qtag. if the D-S-C bytes indicate a SNAP frame then
          // we can handle it

          etsfd=reinterpret_cast<EthernetTaggedSnapFrameData *>(fd.buffer);

          if(etsfd->eth_dsap==0xaa && etsfd->eth_ssap==0xaa && etsfd->eth_control==0x03) {

            // set up the values from the SNAP frame with qtag

            ef.protocol=NetUtil::ntohs(etsfd->eth_etherType);
            ef.payload=etsfd->eth_data;
            ef.payloadLength=fd.length-offsetof(EthernetTaggedSnapFrameData,eth_data);
          }
          else
            return this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_UNSUPPORTED_802_3_FRAME_FORMAT);
        }
        else {

          // it's a tagged ethernet v2 frame

          ef.payload=eftd->eth_data;
          ef.payloadLength=fd.length-offsetof(EthernetTaggedFrameData,eth_data);
        }
      }
      else {

        // no tag, get the protocol - which might be a length if this is a SNAP frame

        ef.protocol=NetUtil::ntohs(efd->eth_etherType);

        if(ef.protocol<0x0600) {

          EthernetSnapFrameData *esfd;

          // this is an 802.3 frame. if the D-S-C bytes indicate a SNAP frame then
          // we can handle it

          esfd=reinterpret_cast<EthernetSnapFrameData *>(fd.buffer);

          if(esfd->eth_dsap==0xaa && esfd->eth_ssap==0xaa && esfd->eth_control==0x03) {

            // set up the values from the SNAP frame with no qtag

            ef.protocol=NetUtil::ntohs(esfd->eth_etherType);
            ef.payload=esfd->eth_data;
            ef.payloadLength=fd.length-offsetof(EthernetSnapFrameData,eth_data);
          }
          else
            return this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_UNSUPPORTED_802_3_FRAME_FORMAT);
        }
        else {

          // it's an ethernet v2 frame which we can handle

          ef.payload=efd->eth_data;
          ef.payloadLength=fd.length-offsetof(EthernetTaggedFrameData,eth_data);
        }
      }

      // it's OK

      return true;
    }


    /**
     * Send a frame over the ethernet. This high level method validates the net buffer
     * and tries to call the frame sender. If the MAC is busy and we are not in an IRQ context
     * then we'll retry sending for a configurable number of milliseconds.
     *
     * @param ned The event containing the data for the request
     */

    void MacBase::onSend(NetEventDescriptor& ned) {

      // must be an ethernet send request

      if(ned.eventType!=NetEventDescriptor::NetEventType::ETHERNET_TRANSMIT_REQUEST)
        return;

      EthernetTransmitRequestEvent& event=static_cast<EthernetTransmitRequestEvent&>(ned);
      EthernetFrameData *efd;

      // we cannot transmit data out of flash memory because the flash banks are
      // not connected to the Ethernet DMA bus on the STM32. More's the pity.

      uint32_t ub=reinterpret_cast<uint32_t>(event.networkBuffer->getUserBuffer());

      if(ub && IS_FLASH_ADDRESS(ub)) {
        delete event.networkBuffer;
        this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_NO_FLASH_DATA);
        return;
      }

      // the NetBuffer needs to get an ethernet header

      efd=reinterpret_cast<EthernetFrameData *>(event.networkBuffer->moveWritePointerBack(getDatalinkTransmitHeaderSize()));

      efd->eth_destinationAddress=event.macAddress;
      efd->eth_sourceAddress=_params.mac_address;
      efd->eth_etherType=NetUtil::htons(static_cast<uint16_t>(event.etherType));

      uint32_t now=MillisecondTimer::millis();

      while(!sendBuffer(event.networkBuffer)) {

        if(errorProvider.isLastError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_BUSY)) {

          // DMA still has our TX descriptor. If we're not running in an IRQ context then we
          // can wait to see if it frees up

          if(Nvic::isAnyIrqActive()) {
            delete event.networkBuffer;
            return;
          }

          if(MillisecondTimer::hasTimedOut(now,_params.mac_txWaitMillis)) {
            delete event.networkBuffer;
            return;
          }
        }
        else {
          delete event.networkBuffer;
          return;         // other error
        }
      }

      // it was sent and the net buffer will be deleted when the TX interrupt is processed

      event.succeeded=true;
    }


    /**
     * Send the content of a NetBuffer via DMA. One NetBuffer contains exactly one frame which may be
     * in one or two buffers. When two buffers are used typically the first buffer contain all the
     * metadata in the form of headers and the second buffer is the user data to send.
     *
     * @param nb The buffer to send.
     * @return true if it worked
     */

    bool MacBase::sendBuffer(NetBuffer *nb) {

      // ensure we cannot be interrupted either by an IRQ if we are normal flow
      // or by a higher priority interrupt if we are an IRQ

      IrqSuspend suspender;

      // find the next buffer owned by the CPU with NetBuffer == nullptr (TDES0 OWN bit = 0)
      // it's important to consider NetBuffer == nullptr to avoid a race condition with the
      // IRQ handler that cleans up the NetBuffer

      ETH_DMADESCTypeDef& txdesc(_transmitDmaDescriptors[_transmitBufferIndex]);

      if((txdesc.Status & ETH_DMATxDesc_OWN)!=0 || _transmitNetBuffers[_transmitBufferIndex]!=nullptr)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_BUSY);

      // clear out the buffer1 and buffer2 size bits in TDES1

      txdesc.ControlBufferSize&=~(ETH_DMATxDesc_TBS2 | ETH_DMATxDesc_TBS1);

      // set up buffer1

      txdesc.Buffer1Addr=reinterpret_cast<uint32_t>(nb->getInternalBuffer());
      txdesc.ControlBufferSize|=nb->getInternalBufferSize();

      if(nb->getUserBufferSize()>0) {

        // there are two buffers, headers go out first followed by the user buffer
        // check that we will not exceed the MTU

        if(nb->getUserBufferSize()+nb->getInternalBufferSize()>_params.mac_mtu)
          return this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_TOO_BIG);

        // set up buffer2

        txdesc.Buffer2NextDescAddr=reinterpret_cast<uint32_t>(nb->getUserBuffer());
        txdesc.ControlBufferSize|=nb->getUserBufferSize() <<  16;
      }
      else if(nb->getInternalBufferSize()>_params.mac_mtu)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,E_TOO_BIG);

      // this is the first and the last frame and DMA owns it now. OWN must be set before
      // the netbuffer pointer is inserted to avoid a race condition with the cleanup
      // code in the transmit interrupt handler.

      txdesc.Status&=~(ETH_DMATxDesc_ChecksumIPV4Header | ETH_DMATxDesc_ChecksumTCPUDPICMPFull | ETH_DMATxDesc_ChecksumByPass);

      if(nb->getChecksumRequest()==DatalinkChecksum::IP_HEADER)
        txdesc.Status|=ETH_DMATxDesc_ChecksumIPV4Header;
      else if(nb->getChecksumRequest()==DatalinkChecksum::IP_HEADER_AND_PROTOCOL)
        txdesc.Status|=ETH_DMATxDesc_ChecksumTCPUDPICMPFull;
      else
        txdesc.Status|=ETH_DMATxDesc_ChecksumByPass;

      txdesc.Status|=ETH_DMATxDesc_LS | ETH_DMATxDesc_FS | ETH_DMATxDesc_OWN;

      // set up the NetBuffer pointer in the transmit buffers array

      _transmitNetBuffers[_transmitBufferIndex]=nb;

      // move to the next, or back to the first

      if(_transmitBufferIndex==_params.mac_transmitBufferCount-1)
        _transmitBufferIndex=0;
      else
        _transmitBufferIndex++;

      // trigger DMA to poll for transmit buffers

      if((ETH->DMASR & ETH_DMASR_TBUS)!=0) {
        ETH->DMASR=ETH_DMASR_TBUS;
        ETH->DMATPDR=0;               // poll demand register
      }

      // done

      return true;
    }


    /**
     * A frame has been transmitted, release memory used by the NetBuffer
     * that held the memory
     */

    void MacBase::handleTransmitInterrupt() {

      uint16_t i;
      ETH_DMADESCTypeDef *txbuf;
      NetBuffer *nb;

      // loop over the small number of tx buffers

      txbuf=_transmitDmaDescriptors.get();
      for(i=0;i<_params.mac_transmitBufferCount;i++) {

        // if the CPU owns the descriptor and a buffer is present then it's finished
        // free the buffer and clear it out

        if((txbuf->Status & ETH_DMATxDesc_OWN)==0 && _transmitNetBuffers[i]!=nullptr) {

          // send a notification that a NetBuffer is being cleaned up. This can be used by
          // the receiver to synchronise frame send requests with the frame actually being
          // transmitted

          nb=_transmitNetBuffers[i];
          this->NetworkNotificationEventSender.raiseEvent(DatalinkFrameSentEvent(*nb));

          // if this is the last in a sequence of fragmented packets then there will be a referenced
          // netbuffer that is serving to hold the jumbo packet's memory in scope while the fragments
          // got tx'd. now that's done we're safe to delete it and we must also notify it upwards
          // because that's the buffer that anything waiting on will recognise.

          if(nb->getReference())
            this->NetworkNotificationEventSender.raiseEvent(DatalinkFrameSentEvent(*(nb->getReference())));

          // clean up the buffer

          delete _transmitNetBuffers[i];
          _transmitNetBuffers[i]=nullptr;
        }
      }
    }


    /**
     * Handle an error interrupt from the DMA
     * @param dmaStatus The DMA status register with the interrupt flags in it
     */

    void MacBase::handleErrorInterrupt(uint32_t dmaStatus) {

      int context;

      if((dmaStatus & ETH_DMA_FLAG_TPS)!=0)
        context=E_TRANSMIT_PROCESS_STOPPED;
      else if((dmaStatus & ETH_DMA_FLAG_TJT)!=0)
        context=E_TRANSMIT_JABBER_TIMEOUT;
      else if((dmaStatus & ETH_DMA_FLAG_RO)!=0)
        context=E_RECEIVE_OVERFLOW;
      else if((dmaStatus & ETH_DMA_FLAG_TU)!=0)
        context=E_TRANSMIT_UNDERFLOW;
      else if((dmaStatus & ETH_DMA_FLAG_RBU)!=0)
        context=E_RECEIVE_BUFFER_UNAVAILABLE;
      else if((dmaStatus & ETH_DMA_FLAG_RPS)!=0)
        context=E_RECEIVE_PROCESS_STOPPED;
      else if((dmaStatus & ETH_DMA_FLAG_RWT)!=0)
        context=E_RECEIVE_WATCHDOG_TIMEOUT;
      else if((dmaStatus & ETH_DMA_FLAG_FBE)!=0)
        context=E_FATAL_BUS_ERROR;
      else
        context=E_UNSPECIFIED;

      this->setError(ErrorProvider::ERROR_PROVIDER_NET_MAC,context);
    }
  }
}


#endif
