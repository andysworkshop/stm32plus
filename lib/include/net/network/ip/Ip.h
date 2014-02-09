/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Network layer feature that implements IP version 4.
     */

    DECLARE_EVENT_SIGNATURE(IpReceive,void (IpPacketEvent&));

    template<class TDatalinkLayer,class... Features>
    class Ip : public virtual TDatalinkLayer,
               public IpPorts,
               public Features... {

      public:

        /**
         * Error codes
         */

        enum {
          E_REASSEMBLY_FAILURE = 1,
          E_FRAGMENTATION_FAILED,
          E_ARP_LOOKUP_FAILED,
          E_OUT_OF_MEMORY,
          E_UNCONFIGURED
        };

        DECLARE_EVENT_SOURCE(IpReceive);

        /**
         * Parameters for tuning the IP protocol handler
         */

        struct Parameters : IpPorts::Parameters,
                            Features::Parameters... {

          uint8_t ip_initialTtl;                    ///< TTL value inserted into new IP packets. The default is 64.

          /**
           * Constructor, set up parameters
           */

          Parameters() {
            ip_initialTtl=64;
          }
        };

      protected:
        IpAddress _myIpAddress;
        IpSubnetMask _mySubnetMask;
        MacAddress _myMacAddress;
        uint8_t _initialTtl;

      protected:
        void handleAddressMappingEvent(const MacAddress& mac,const IpAddress& ipAddress);
        bool canAcceptPacket(const IpAddress& destinationAddress) const;

        void onReceive(NetEventDescriptor& ned);
        void onSend(NetEventDescriptor& ned);
        void onNotification(NetEventDescriptor& ned);

        void setCommonTransmitHeaderValues(IpPacketHeader& header,IpTransmitRequestEvent& txevent);
        void sendToLocalhost(IpTransmitRequestEvent& txevent);

      public:
        bool initialise(Parameters&);
        bool startup();

        const IpAddress& getIpAddress() const;
        constexpr uint32_t getIpTransmitHeaderSize() const;
    };


    /**
     * Define a type for the common case of not requiring support for packet
     * fragmentation and reassembly
     */

    template<class TDatalinkLayer>
    using DefaultIp=Ip<TDatalinkLayer,IpDisablePacketFragmentFeature,IpDisablePacketReassemblerFeature>;


    /**
     * Define types for the less usual cases of packet fragmentation, reassembly and both
     */

    template<class TDatalinkLayer>
    using IpWithOutboundFragmentation=Ip<TDatalinkLayer,IpPacketFragmentFeature,IpDisablePacketReassemblerFeature>;

    template<class TDatalinkLayer>
    using IpWithInboundReassembly=Ip<TDatalinkLayer,IpDisablePacketFragmentFeature,IpPacketReassemblerFeature>;

    template<class TDatalinkLayer>
    using IpWithFragmentationAndReassembly=Ip<TDatalinkLayer,IpPacketFragmentFeature,IpPacketReassemblerFeature>;


    /**
     * Initialise the protocol.
     * @param params The parameters
     * @return true if it worked
     */

    template<class TDatalinkLayer,class... Features>
    inline bool Ip<TDatalinkLayer,Features...>::initialise(Parameters& params) {

      // initialise the ports class

      if(!IpPorts::initialise(params,*this))
        return false;

      // initialise the features list

      if(!RecursiveBoolInitWithParamsAndNetworkUtilityObjects<Ip,Features...>::tinit(this,params,*this))
        return false;

      // remember parameters

      _initialTtl=params.ip_initialTtl;

      // subscribe to send/receive/notify events from the network

      this->NetworkReceiveEventSender.insertSubscriber(NetworkReceiveEventSourceSlot::bind(this,&Ip<TDatalinkLayer,Features...>::onReceive));
      this->NetworkSendEventSender.insertSubscriber(NetworkSendEventSourceSlot::bind(this,&Ip<TDatalinkLayer,Features...>::onSend));
      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&Ip<TDatalinkLayer,Features...>::onNotification));

      return true;
    }


    /**
     * Startup the class.
     * @param params The parameters
     * @return true if it worked
     */

    template<class TDatalinkLayer,class... Features>
    inline bool Ip<TDatalinkLayer,Features...>::startup() {

      // startup the features list

      return RecursiveBoolStartup<Ip,Features...>::tstartup(this);;
    }


    /**
     * Get the size of the Tx header
     * @return 20 bytes. We don't set extended options.
     */

    template<class TDatalinkLayer,class... Features>
    inline constexpr uint32_t Ip<TDatalinkLayer,Features...>::getIpTransmitHeaderSize() const {
      return IpPacketHeader::getNoOptionsHeaderSize();
    }


    /**
     * Notification from our subscription to notification events
     * @param ned The event descriptor
     */

    template<class TDatalinkLayer,class... Features>
    __attribute__((noinline)) inline void Ip<TDatalinkLayer,Features...>::onNotification(NetEventDescriptor& ned) {

      // if it's a new IP address then we are interested

      if(ned.eventType==NetEventDescriptor::NetEventType::IP_ADDRESS_ANNOUNCEMENT)
        _myIpAddress=static_cast<IpAddressAnnouncementEvent&>(ned).ipAddress;
      else if(ned.eventType==NetEventDescriptor::NetEventType::MAC_ADDRESS_ANNOUNCEMENT)
        _myMacAddress=static_cast<MacAddressAnnouncementEvent&>(ned).macAddress;
      else if(ned.eventType==NetEventDescriptor::NetEventType::SUBNET_MASK_ANNOUNCEMENT)
        _mySubnetMask=static_cast<IpSubnetMaskAnnouncementEvent&>(ned).subnetMask;
    }


    /**
     * Notification from our subscription to the datalink layer's receive events. This is IRQ code.
     * @param ned The event descriptor
     */

    template<class TDatalinkLayer,class... Features>
    __attribute__((noinline)) inline void Ip<TDatalinkLayer,Features...>::onReceive(NetEventDescriptor& ned) {

      IpPacketHeader *header;

      // check if it's for us

      if(ned.eventType!=NetEventDescriptor::NetEventType::DATALINK_FRAME)
        return;

      DatalinkFrame& frame=static_cast<DatalinkFrameEvent&>(ned).datalinkFrame;

      // must be an IP frame

      if(frame.protocol!=static_cast<uint16_t>(EtherType::IP))
        return;

      // IP packets cannot be shorter than 20 bytes

      if(frame.payloadLength<20)
        return;

      // quickly check the version and get out if it's not IP

      header=reinterpret_cast<IpPacketHeader *>(frame.payload);
      if((header->ip_hdr_version & 0xf0)!=0x40)
        return;

      // we accept the packet if the destination address...
      // 1. Is our unicast address
      // 2. Is the global broadcast address
      // 3. Is the local subnet broadcast address
      // 4. Is a multicast group to which we are subscribed

      if(!canAcceptPacket(header->ip_destinationAddress))
        return;

      // now construct the packet structure

      IpPacket packet;

      // for performance reasons the header fields are not deconstructed here. they can
      // be fetched on demand for anything that needs them.

      packet.headerLength=(header->ip_hdr_version & 0x0f)*4;
      packet.header=header;
      packet.payload=reinterpret_cast<uint8_t *>(((uint32_t)header)+packet.headerLength);
      packet.payloadLength=NetUtil::ntohs(header->ip_hdr_length)-packet.headerLength;

      // if the packet came from ethernet then we notify that there is a potentially
      // new address mapping that can be cached

      if(frame.frameSource==DatalinkFrame::FrameSource::ETHERNET_FRAME)
        handleAddressMappingEvent(*static_cast<EthernetFrame&>(frame).sourceMac,packet.header->ip_sourceAddress);

      // if the packet is fragmented, handle it

      if(packet.isFragmented()) {

        IpFragmentedPacket *fp;

        if(!this->ip_handleFragment(packet,fp)) {

          // there was a failure to handle the packet fragment, notify observers and finish

          this->setError(errorProvider.getProvider(),errorProvider.getCode(),errorProvider.getCause());
          return;
        }

        // if this is the last fragment then the reassembled packet is the one to use and it can be free'd
        // after we've notified upwards

        if(fp->isComplete()) {

          // the header on the this incoming packet is fine but the payload is the reassembled packet

          packet.payload=fp->packet;
          packet.payloadLength=fp->packetLength;

          // notify and free

          IpReceiveEventSender.raiseEvent(IpPacketEvent(packet));
          this->ip_freePacket(fp);
        }
      }
      else {

        // notify the observers of the incoming packet

        IpReceiveEventSender.raiseEvent(IpPacketEvent(packet));
      }
    }


    /**
     * Check if we can accept this packet
     * @param destinationAddress Where the packet was addressed to
     * @return true if we can accept it
     */

    template<class TDatalinkLayer,class... Features>
    inline bool Ip<TDatalinkLayer,Features...>::canAcceptPacket(const IpAddress& destinationAddress) const {

      // must check this first because DHCP replies come back as broadcasts at startup
      // while we are unconfigured

      if(destinationAddress.isBroadcast() || destinationAddress.isAllHostsMulticastGroup())
        return true;

      // further checks need our address

      if(!_myIpAddress.isValid())
        return false;

      // if it's our address then OK

      if(destinationAddress==_myIpAddress)
        return true;

      // further checks need our subnet mask

      if(!_mySubnetMask.isValid())
        return false;

      // check for the local broadcast address

      if(_mySubnetMask.isBroadcastAddress(_myIpAddress))
        return true;

      // TODO: support user-joined multicast groups

      return false;
    }


    /**
     * Notification from our subscription to the network's send event
     * @param ned The event descriptor
     */

    template<class TDatalinkLayer,class... Features>
    __attribute__((noinline)) inline void Ip<TDatalinkLayer,Features...>::onSend(NetEventDescriptor& ned) {

      uint16_t packetSize;

      // must be a send event

      if(ned.eventType!=NetEventDescriptor::NetEventType::IP_TRANSMIT_REQUEST)
        return;

      IpTransmitRequestEvent& txevent(static_cast<IpTransmitRequestEvent&>(ned));

      // special case for the local class A network

      if(txevent.destinationIpAddress.isLocalNetwork()) {
        sendToLocalhost(txevent);
        txevent.succeeded=true;
        return;
      }

      // we can't do anything without a MAC.

      if(!_myMacAddress.isValid()) {
        this->setError(ErrorProvider::ERROR_PROVIDER_NET_IP,E_UNCONFIGURED);
        return;
      }

      // we can only do without an IP address if the destination is 255.255.255.255
      // because it's going to be the DHCP client

      if(!_myIpAddress.isValid() && !txevent.destinationIpAddress.isBroadcast()) {
        this->setError(ErrorProvider::ERROR_PROVIDER_NET_IP,E_UNCONFIGURED);
        return;
      }

      // we need the MAC address of the destination. if we came here from an IRQ (a received frame)
      // then it will already have been put into the ARP cache on the way up the stack. as well as
      // being efficient this is necessary because we cannot do tx-wait-rx while in an ethernet rx IRQ.

      ArpMappingRequestEvent arpRequest(txevent.destinationIpAddress);

      this->NetworkNotificationEventSender.raiseEvent(arpRequest);

      // did it work?

      if(!arpRequest.found) {
        this->setError(ErrorProvider::ERROR_PROVIDER_NET_IP,E_ARP_LOOKUP_FAILED);
        return;
      }

      // get the total IP packet size and check if we must fragment

      packetSize=getIpTransmitHeaderSize()+txevent.networkBuffer->getSizeFromWritePointerToEnd()+txevent.networkBuffer->getUserBufferSize();
      if(packetSize>this->getDatalinkMtuSize()) {

        NetBuffer **outputBuffers;
        uint16_t i,outputBufferCount;

        // create fragments

        if(!this->ip_createFragments(txevent.protocol,
                                      _myIpAddress,
                                      txevent.destinationIpAddress,
                                      this->getDatalinkMtuSize(),
                                      this->getDatalinkTransmitHeaderSize(),
                                      txevent.networkBuffer,
                                      outputBuffers,
                                      outputBufferCount)) {

          this->setError(ErrorProvider::ERROR_PROVIDER_NET_IP,E_FRAGMENTATION_FAILED);
          return;
        }

        // send the fragments

        for(i=0;i<outputBufferCount;i++) {

          setCommonTransmitHeaderValues(
              *reinterpret_cast<IpPacketHeader *>(outputBuffers[i]->getWritePointer()),
              txevent);

          // send the fragment. fragmented packets cannot have auto protocol checksum calculation because
          // they are incomplete.

          EthernetTransmitRequestEvent etre(outputBuffers[i],
                                            arpRequest.macAddress,
                                            EtherType::IP,
                                            DatalinkChecksum::IP_HEADER);

          // raise the event and fail if it failed (the link layer will already have set an error)

          this->NetworkSendEventSender.raiseEvent(etre);
          if(!etre.succeeded) {
            free(outputBuffers);
            return;
          }
        }

        // free memory used to hold the array of netbuffers who's ownership is now transferred to
        // the link layer

        free(outputBuffers);
      }
      else {

        // create the IP header in the netbuffer

        IpPacketHeader *header=reinterpret_cast<IpPacketHeader *>(txevent.networkBuffer->moveWritePointerBack(getIpTransmitHeaderSize()));

        setCommonTransmitHeaderValues(*header,txevent);

        // these values are not common between fragmented and non-fragmented

        header->ip_hdr_length=NetUtil::htons(static_cast<uint16_t>(txevent.networkBuffer->getSizeFromWritePointerToEnd()+txevent.networkBuffer->getUserBufferSize()));
        header->ip_hdr_identification=0;
        header->ip_hdr_flagsAndOffset=0;                                  // we will not fragment

        // cool, now we have enough info to ask the datalink layer to send the packet

        EthernetTransmitRequestEvent etre(txevent.networkBuffer,
                                          arpRequest.macAddress,
                                          EtherType::IP,
                                          DatalinkChecksum::IP_HEADER_AND_PROTOCOL);

        // raise the event

        this->NetworkSendEventSender.raiseEvent(etre);
        if(!etre.succeeded)
          return;             // error code already set by link layer
      }

      // it worked

      txevent.succeeded=true;
    }


    /**
     * Send this event to the localhost. i.e. don't send it at all, just raise a
     * receive event for the NetBuffer then clean it up afterwards. This has the side effect
     * that the receive event is called on the main thread and not an IRQ. That should not
     * make any difference to user code.
     * @param txevent The transmit event
     */

    template<class TDatalinkLayer,class... Features>
    inline void Ip<TDatalinkLayer,Features...>::sendToLocalhost(IpTransmitRequestEvent& txevent) {

      IpPacketHeader header;
      IpPacket packet;
      scoped_array<uint8_t> buffer;
      uint32_t bufferSize;

      // set up the header

      setCommonTransmitHeaderValues(header,txevent);
      packet.header=&header;
      packet.headerLength=IpPacketHeader::getNoOptionsHeaderSize();

      // if the TX payload is in just one of the netbuffer buffers then we can use it
      // without copying it out

      if(txevent.networkBuffer->getInternalBufferSize()!=0 && txevent.networkBuffer->getUserBufferSize()!=0) {

        bufferSize=txevent.networkBuffer->getInternalBufferSize()+txevent.networkBuffer->getUserBufferSize();
        buffer.reset(new uint8_t[bufferSize]);

        if(buffer.get()==nullptr) {
          this->setError(ErrorProvider::ERROR_PROVIDER_NET_IP,E_OUT_OF_MEMORY);
          return;
        }

        // copy the buffers out to a linear memory space

        memcpy(buffer.get(),txevent.networkBuffer->getInternalBuffer(),txevent.networkBuffer->getInternalBufferSize());
        memcpy(buffer.get()+txevent.networkBuffer->getInternalBufferSize(),txevent.networkBuffer->getUserBuffer(),txevent.networkBuffer->getUserBufferSize());

        packet.payload=buffer.get();
        packet.payloadLength=bufferSize;
      }
      else if(txevent.networkBuffer->getUserBufferSize()==0) {

        // only internal buffer is present, reference it directly

        packet.payload=reinterpret_cast<uint8_t *>(txevent.networkBuffer->getInternalBuffer());
        packet.payloadLength=txevent.networkBuffer->getInternalBufferSize();
      }
      else {

        // only user buffer is present (not likely), reference it directly

        packet.payload=(uint8_t *)txevent.networkBuffer->getUserBuffer();
        packet.payloadLength=txevent.networkBuffer->getUserBufferSize();
      }

      // raise the event

      this->NetworkReceiveEventSender.raiseEvent(IpPacketEvent(packet));

      // the txevent netbuffer is dead

      delete txevent.networkBuffer;
    }


    /**
     * Set the common values in the IP header for a transmitted packet. Common means common to fragmented
     * and non-fragmented packets
     * @param header The header structure
     * @param txevent The transmit event
     */

    template<class TDatalinkLayer,class... Features>
    inline void Ip<TDatalinkLayer,Features...>::setCommonTransmitHeaderValues(IpPacketHeader& header,IpTransmitRequestEvent& txevent) {

      header.ip_hdr_version=0x45;                                     // uint8_t
      header.ip_hdr_typeOfService=0;
      header.ip_hdr_ttl=txevent.ttl ? txevent.ttl : _initialTtl;        // uint8_t
      header.ip_hdr_protocol=txevent.protocol;                          // uint8_t
      header.ip_hdr_checksum=0;                                       // MAC will calculate this
      header.ip_sourceAddress=_myIpAddress;
      header.ip_destinationAddress=txevent.destinationIpAddress;
    }


    /**
     * When a frame has been identified as coming from ethernet we notify that address
     * association so that the ARP cache can get updated. This means that'll be ready
     * when a response is needed. This is IRQ code.
     * @param mac The mac address
     * @param ipAddress The IP address
     */

    template<class TDatalinkLayer,class... Features>
    inline void Ip<TDatalinkLayer,Features...>::handleAddressMappingEvent(const MacAddress& mac,const IpAddress& ipAddress) {
      this->NetworkNotificationEventSender.raiseEvent(IpAddressMappingEvent(mac,ipAddress));
    }


    /**
     * Get my IP address
     * @return My IP address
     */

    template<class TDatalinkLayer,class... Features>
    inline const IpAddress& Ip<TDatalinkLayer,Features...>::getIpAddress() const {
      return _myIpAddress;
    }
  }
}
