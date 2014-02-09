/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * ICMPv4 protocol implementation. We support automatic response to echo-request
     * packets.
     */

    template<class TNetworkLayer>
    class Icmp : public virtual TNetworkLayer {

      public:

        /**
         * Parameters class
         */

        struct Parameters {
          bool icmp_enableEchoReply;      ///< true to respond to ping requests, default is true

          /**
           * Constructor: create default settings
           */

          Parameters() {
            icmp_enableEchoReply=true;
          }
        };

      protected:
        Parameters _params;

      protected:
        bool handleEchoReply(IcmpEchoRequest *echoRequest,IpPacket& ipPacket);
        void onReceive(IpPacketEvent& ned);
        void onSend(NetEventDescriptor& ned);

      public:
        bool initialise(const Parameters& params);
        bool startup();

        bool icmpSendEchoRequest(const IpAddress& ipaddress,
                                  uint16_t sequenceNumber,
                                  const uint8_t *data,
                                  uint32_t dataSize,
                                  uint8_t ttl=0);
    };


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TNetworkLayer>
    inline bool Icmp<TNetworkLayer>::initialise(const Parameters& params) {

      _params=params;

      // subscribe for receive events from the IP implementation

      this->IpReceiveEventSender.insertSubscriber(IpReceiveEventSourceSlot::bind(this,&Icmp<TNetworkLayer>::onReceive));

      // subscribe to send events from the stack

      this->NetworkSendEventSender.insertSubscriber(NetworkSendEventSourceSlot::bind(this,&Icmp<TNetworkLayer>::onSend));

      return true;
    }


    /**
     * Startup the class
     * @return true if it worked
     */

    template<class TNetworkLayer>
    inline bool Icmp<TNetworkLayer>::startup() {
      return true;
    }


    /**
     * Notification from our subscription to the network's send event
     * @param ned The event descriptor
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Icmp<TNetworkLayer>::onSend(NetEventDescriptor& ned) {

      NetBuffer *nb;

      // check the event type

      if(ned.eventType!=NetEventDescriptor::NetEventType::ICMP_TRANSMIT_REQUEST)
        return;

      IcmpTransmitRequestEvent& icmptre(static_cast<IcmpTransmitRequestEvent&>(ned));

      // create a net buffer for the packet

      nb=new NetBuffer(this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize(),
                       0,
                       &icmptre.packet,
                       icmptre.packetSize);

      // raise a transmit event for the IP layer to pick up


      IpTransmitRequestEvent iptre(
          nb,
          icmptre.destinationIpAddress,
          IpProtocol::ICMP);

      this->NetworkSendEventSender.raiseEvent(iptre);
      icmptre.succeeded=iptre.succeeded;
    }


    /**
     * Receive event notification from the stack
     * @param ipe The IP packet event
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Icmp<TNetworkLayer>::onReceive(IpPacketEvent& ipe) {

      IpPacket& ipPacket(ipe.ipPacket);

      // protocol/type of service == 1/0

      if(ipPacket.header->ip_hdr_protocol!=IpProtocol::ICMP || ipPacket.header->ip_hdr_typeOfService!=0)
        return;

      IcmpPacket *icmpPacket=reinterpret_cast<IcmpPacket *>(ipPacket.payload);

      // is it an echo request

      if(IcmpEchoRequest::isPacket(*icmpPacket)) {

        // are we supporting replies to echo (ping) requests?

        if(_params.icmp_enableEchoReply)
          handleEchoReply(reinterpret_cast<IcmpEchoRequest *>(icmpPacket),ipPacket);
      }

      // all packets are pushed up the stack in the form of events including the echo
      // request even though it may be in the process of having a reply packet pushed out

      this->NetworkReceiveEventSender.raiseEvent(IcmpPacketEvent(*icmpPacket,ipPacket));
    }


    /**
     * Handle the echo reply.
     * @param echoRequest The echo request coming in.
     */

    template<class TNetworkLayer>
    inline bool Icmp<TNetworkLayer>::handleEchoReply(IcmpEchoRequest *echoRequest,IpPacket& ipPacket) {

      NetBuffer *nb;
      uint32_t requestDataSize;

      // calculate the request data size

      requestDataSize=ipPacket.payloadLength-IcmpEchoRequest::getHeaderSize();

      // create a net buffer for the echo reply
      // replies are always coming from an incoming frame IRQ
      // cannot use the incoming request data in-place because it's going
      // to go out of scope before the frame is transmitted

      nb=new NetBuffer(this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize(),
                       IcmpEchoReply::getHeaderSize()+requestDataSize);

      // copy over the data from the request, if there was any - there doesn't have to be

      if(requestDataSize) {
        uint8_t *replyData=reinterpret_cast<uint8_t *>(nb->moveWritePointerBack(requestDataSize));
        memcpy(replyData,echoRequest->icmp_data,requestDataSize);
      }

      // move back to the location of the ICMP header and set it up

      IcmpEchoReply *reply=reinterpret_cast<IcmpEchoReply *>(nb->moveWritePointerBack(IcmpEchoReply::getHeaderSize()));
      reply->initialise();
      reply->icmp_identifier=echoRequest->icmp_identifier;
      reply->icmp_sequenceNumber=echoRequest->icmp_sequenceNumber;

      // raise a transmit event for the IP layer to pick up

      IpTransmitRequestEvent iptre(
          nb,
          ipPacket.header->ip_sourceAddress,
          IpProtocol::ICMP);

      this->NetworkSendEventSender.raiseEvent(iptre);
      return iptre.succeeded;
    }


    /**
     * Create and send an echo request to the given IP address.
     * @param ipaddress Where to send the echo request
     * @param data The data to send in the packet
     * @param dataSize The number of bytes to send
     * @param ttl TTL for the IP packet. Default of zero means use value from initialisation parameters.
     */

    template<class TNetworkLayer>
    inline bool Icmp<TNetworkLayer>::icmpSendEchoRequest(
            const IpAddress& ipaddress,
            uint16_t sequenceNumber,
            const uint8_t *data,
            uint32_t dataSize,
            uint8_t ttl) {

      NetBuffer *nb;
      uint32_t requestDataSize;

      // calculate the request data size

      requestDataSize=IcmpEchoRequest::getHeaderSize()+dataSize;

      // create a net buffer for the echo reply
      // replies are always coming from an incoming frame IRQ
      // cannot use the incoming request data in-place because it's going
      // to go out of scope before the frame is transmitted

      nb=new NetBuffer(this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize(),
                       requestDataSize,
                       nullptr);

      // copy over the data for the request

      uint8_t *requestData=reinterpret_cast<uint8_t *>(nb->moveWritePointerBack(dataSize));
      memcpy(requestData,data,dataSize);

      // move back to the location of the ICMP header and set it up

      IcmpEchoRequest *request=reinterpret_cast<IcmpEchoRequest *>(nb->moveWritePointerBack(IcmpEchoRequest::getHeaderSize()));
      request->initialise();
      request->icmp_identifier=0x55;        // constant to indicate we sent it
      request->icmp_sequenceNumber=NetUtil::htons(sequenceNumber);

      // raise a transmit event for the IP layer to pick up

      IpTransmitRequestEvent iptre(
          nb,
          ipaddress,
          IpProtocol::ICMP,
          ttl);

      this->NetworkSendEventSender.raiseEvent(iptre);
      return iptre.succeeded;
    }
  }
}
