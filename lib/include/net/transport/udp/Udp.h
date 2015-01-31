/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    DECLARE_EVENT_SIGNATURE(UdpReceive,void (UdpDatagramEvent&));

    /**
     * Implementation of the UDP protocol over IP. Datagrams are received asynchronously from the IP
     * layer and passed on to the upper layers. Functionality is provided for sending and receiving
     * datagrams synchronously to the caller.
     */

    template<class TNetworkLayer>
    class Udp : public virtual TNetworkLayer {

      public:

        /**
         * Parameters class
         */

        struct Parameters {

          bool udp_sendPortUnreachable;     ///< datagrams sent to ports with no handler will get an ICMP error message (if ICMP is configured in). default is true

          Parameters() {
            udp_sendPortUnreachable=true;
          }
        };


        /**
         * Error codes
         */

        enum {
          E_TIMED_OUT = 1,    ///< timed out while waiting for data
          E_MSG_SIZE,         ///< data was received, but more is available and has been lost
        };

        DECLARE_EVENT_SOURCE(UdpReceive);

      protected:
        Parameters _params;                           ///< protocol parameters
        volatile bool _awaiting;                      ///< true to indicate we're waiting for receive
        volatile uint16_t _awaitingPort;              ///< port we're waiting on
        volatile void *_awaitingBuffer;               ///< buffer to copy incoming data into
        volatile uint16_t *_awaitingBufferSize;       ///< buffer size, updated with actual value
        volatile uint16_t _awaitingDatagramSize;      ///< the actual size received
        volatile IpPacketHeader _ipPacketHeader;      ///< the underlying IP packet header

      protected:
        void onReceive(IpPacketEvent& ned);
        void onNotification(NetEventDescriptor& ned);

        volatile NetBuffer *_waitForThisBuffer;

      public:
        bool initialise(const Parameters& params);
        bool startup();

        bool udpSend(const IpAddress& ipaddress,
                     uint16_t sourcePortNumber,
                     uint16_t destinationPortNumber,
                     const void *data,
                     uint16_t dataSize,
                     bool async,
                     uint32_t transmitTimeout);

        // synchronous receive functions

        bool udpReceive(uint16_t portNumber,void *buffer,uint16_t& size,uint32_t receiveTimeout=0);
        const volatile IpPacketHeader& udpGetIpPacketHeader() const;
        const volatile IpAddress& udpGetRemoteAddress() const;
    };


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true if it worked
     */

    template<class TNetworkLayer>
    inline bool Udp<TNetworkLayer>::initialise(const Parameters& params) {

      // remember parameters

      _params=params;

      // not waiting for anything

      _waitForThisBuffer=nullptr;
      _awaiting=false;

      // subscribe for receive and notification events from the IP implementation

      this->IpReceiveEventSender.insertSubscriber(IpReceiveEventSourceSlot::bind(this,&Udp<TNetworkLayer>::onReceive));
      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&Udp<TNetworkLayer>::onNotification));

      return true;
    }


    /**
     * Startup the class
     * @return true if it worked
     */

    template<class TNetworkLayer>
    inline bool Udp<TNetworkLayer>::startup() {
      return true;
    }


    /**
     * Receive event notification from the stack
     * @param ipe The IP packet event
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Udp<TNetworkLayer>::onReceive(IpPacketEvent& ipe) {

      bool handled;

      // is this UDP?

      if(ipe.ipPacket.header->ip_hdr_protocol!=IpProtocol::UDP)
        return;

      // notify upwards that a UDP datagram has arrived

      UdpDatagram *datagram=reinterpret_cast<UdpDatagram *>(ipe.ipPacket.payload);

      // are we waiting for a datagram?

      if(_awaiting) {

        // the destination port must match

        if(NetUtil::ntohs(datagram->udp_destinationPort)==_awaitingPort) {

          // copy the incoming data and release the waiting code

          _awaitingDatagramSize=NetUtil::ntohs(datagram->udp_length)-UdpDatagram::getHeaderSize();
          *_awaitingBufferSize=std::min<uint16_t>((uint16_t)_awaitingDatagramSize,(uint16_t)*_awaitingBufferSize);
          memcpy(const_cast<void *>(_awaitingBuffer),datagram->udp_data,*_awaitingBufferSize);
          const_cast<IpPacketHeader&>(_ipPacketHeader)=*(ipe.ipPacket.header);     // struct copy

          _awaiting=false;
          handled=true;
        }
        else
          handled=false;
      }
      else
        handled=false;

      // raise the receive event

      UdpDatagramEvent ude(*datagram,ipe.ipPacket);
      UdpReceiveEventSender.raiseEvent(ude);

      // merge the handled flag

      if(!handled)
        handled=ude.handled;

      // if nobody could handle this packet and we are configured to do so then we will
      // raise an ICMP port unreachable event which, if ICMP is configured into the stack,
      // will get sent back to the remote end

      if(!handled &&
          _params.udp_sendPortUnreachable &&
          !ipe.ipPacket.header->ip_destinationAddress.isBroadcast() &&
          !ipe.ipPacket.header->ip_destinationAddress.isMulticastGroup()) {

        uint16_t errorPacketSize;

        // the incoming IP header is of a variable size so allocate memory for a
        // packet large enough for it and the ICMP structure

        IcmpDestinationUnreachable *errorPacket=createIcmpErrorPacket<IcmpDestinationUnreachable>(
            IcmpCode::DESTINATION_PORT_UNREACHABLE,
            ipe.ipPacket,
            errorPacketSize
          );

        // create the ICMP transmit request event

        IcmpTransmitRequestEvent itre(
            *errorPacket,
            errorPacketSize,
            ipe.ipPacket.header->ip_sourceAddress);

        // send it

        this->NetworkSendEventSender.raiseEvent(itre);

        // free the error packet

        free(errorPacket);
      }
    }


    /**
     * Send a UDP datagram. The data must be a single datagram which implies a maximum
     * dataSize of 65507 bytes though in practice you should avoid exceeding the
     * MAC MTU size unless you know that the network layer driver supports
     * packet fragmentation and reassembly.
     *
     * If you set the async flag then you should be aware that dynamic memory is
     * allocated to hold the datagram until transmission is complete so you must have
     * enough SRAM to hold it and any other datagrams pending transmission. If you
     * do not set the async flag then this method blocks until the datagram has been
     * transmitted or the timeout expires. Datagrams transmitted synchronously only
     * use dynamic memory for the datagram and frame header information because the
     * datagram data is transmitted directly from the memory pointer that you supply.
     * Synchronous mode trades network throughput for reduced CPU load and SRAM usage.
     *
     * @param ipaddress The IP address to send to.
     * @param sourcePortNumber The port number that you are sending from, or zero if you don't care
     * @param destinationPortNumber The destination port to send to.
     * @param data The data buffer to send
     * @param dataSize The size of the data buffer
     * @param async true to transmit asynchronously. false to transmit synchronously.
     * @param transmitTimeout In synchronous mode the number of ms to wait for a datagram to be
     * transmitted. Ignored in asychronous mode.
     * @return async mode always returns true. sync mode will return false if the datagram send operation times out.
     */

    template<class TNetworkLayer>
    bool Udp<TNetworkLayer>::udpSend(const IpAddress& ipaddress,
                                      uint16_t sourcePortNumber,
                                      uint16_t destinationPortNumber,
                                      const void *data,
                                      uint16_t dataSize,
                                      bool async,
                                      uint32_t transmitTimeout) {

      NetBuffer *nb;

      // create a net buffer for the echo reply
      // replies are always coming from an incoming frame IRQ
      // cannot use the incoming request data in-place because it's going
      // to go out of scope before the frame is transmitted

      if(async) {

        nb=new NetBuffer(this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize()+UdpDatagram::getHeaderSize(),
                         dataSize,
                         nullptr);

        // copy over the data from the request, if there was any - there doesn't have to be

        if(dataSize) {
          uint8_t *packetData=reinterpret_cast<uint8_t *>(nb->moveWritePointerBack(dataSize));
          memcpy(packetData,data,dataSize);
        }
      }
      else {

        // in this mode the user's data is transmitted in-place after the headers have gone out
        // much larger datagrams are possible in this mode at the expense of throughput

        nb=new NetBuffer(this->getDatalinkTransmitHeaderSize()+this->getIpTransmitHeaderSize()+UdpDatagram::getHeaderSize(),
                         0,
                         data,
                         dataSize);

        // we'll need to wait for this buffer to go out

        _waitForThisBuffer=nb;
      }

      // set up the header

      UdpDatagram *header=reinterpret_cast<UdpDatagram *>(nb->moveWritePointerBack(UdpDatagram::getHeaderSize()));

      header->udp_sourcePort=NetUtil::htons(sourcePortNumber);
      header->udp_destinationPort=NetUtil::htons(destinationPortNumber);
      header->udp_checksum=0;         // will be calculated by the MAC
      header->udp_length=NetUtil::htons(dataSize+UdpDatagram::getHeaderSize());

      // send the datagram

      IpTransmitRequestEvent iptre(
                  nb,
                  ipaddress,
                  IpProtocol::UDP
                );

      this->NetworkSendEventSender.raiseEvent(iptre);

      // check that it was accepted for sending

      if(!iptre.succeeded)
        return false;

      // in sync mode we need to wait for the event raised by the datalink layer that indicates
      // the datagram has gone on to the wire

      if(!async) {

        uint32_t now;

        now=MillisecondTimer::millis();

        while(_waitForThisBuffer) {
          if(MillisecondTimer::hasTimedOut(now,transmitTimeout)) {
            _waitForThisBuffer=nullptr;
            return this->setError(ErrorProvider::ERROR_PROVIDER_NET_UDP,E_TIMED_OUT);
          }
        }
      }

      // succeeded

      return true;
    }


    /**
     * Receive a datagram synchronously. This method blocks until data is available or the
     * timeout is hit. Data from the received datagram is stored in 'buffer' up to a maximum
     * of 'size' bytes. The actual amount received is returned in 'size'. If more data is
     * available in the datagram then the method returns false and the error provider code
     * will be E_MSG_SIZE. true is returned if all the data in the datagram could be copied
     * into your 'buffer'
     * @param portNumber The port to wait for data on
     * @param buffer Where to store the data
     * @param[in,out] size The maximum number of bytes to store in 'buffer'. Modified on return
     *   to indicate the actual number of bytes stored.
     * @param receiveTimeout The number of ms to wait for a response. Specify zero to make this a blocking call.
     * @return true if it worked. false on error and also if the datagram was too big to fit in 'buffer'.
     */

    template<class TNetworkLayer>
    bool Udp<TNetworkLayer>::udpReceive(uint16_t portNumber,void *buffer,uint16_t& size,uint32_t receiveTimeout) {

      uint32_t start;

      // set the wait variables

      _awaitingPort=portNumber;
      _awaitingBuffer=buffer;
      _awaitingBufferSize=&size;
      _awaiting=true;

      // wait for something to happen

      start=MillisecondTimer::millis();

      while(_awaiting)
        if(receiveTimeout>0 && MillisecondTimer::hasTimedOut(start,receiveTimeout))
          return this->setError(ErrorProvider::ERROR_PROVIDER_NET_UDP,E_TIMED_OUT);

      // return the correct value

      return _awaitingDatagramSize>size ? this->setError(ErrorProvider::ERROR_PROVIDER_NET_UDP,E_MSG_SIZE) : true;
    }


    /**
     * Get a reference to the IP packet header belonging to the datagram that was last
     * received with the udpReceive() synchronous method. The reference returned by this method
     * is valid until the next call to udpReceive()
     * @return A reference to the IP packet header
     */

    template<class TNetworkLayer>
    const volatile IpPacketHeader& Udp<TNetworkLayer>::udpGetIpPacketHeader() const {
      return _ipPacketHeader;
    }


    /**
     * Convenience function to get the remote address (sender) of the last datagram to be
     * received by the udpReceive() synchronous method. The reference returned by this method
     * is valid until the next call to udpReceive()
     * @return A reference to the IP address
     */

    template<class TNetworkLayer>
    const volatile IpAddress& Udp<TNetworkLayer>::udpGetRemoteAddress() const {
      return _ipPacketHeader.ip_sourceAddress;
    }


    /**
     * Notification event notification from the stack
     * @param ned The network event descriptor
     */

    template<class TNetworkLayer>
    __attribute__((noinline)) inline void Udp<TNetworkLayer>::onNotification(NetEventDescriptor& ned) {

      // we're interested in datalink frames going out when we're in a wait state

      if(_waitForThisBuffer==nullptr || ned.eventType!=NetEventDescriptor::NetEventType::DATALINK_FRAME_SENT)
        return;

      // if it's the buffer we're waiting for then clear it out (wakes up the sender)

      DatalinkFrameSentEvent& dfse(static_cast<DatalinkFrameSentEvent&>(ned));
      if(&dfse.netBuffer==_waitForThisBuffer)
        _waitForThisBuffer=nullptr;
    }
  }
}
