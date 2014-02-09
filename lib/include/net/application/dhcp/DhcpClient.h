/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Application layer feature to provide IP configuration information from
     * a DHCP server. The DHCP protocol is entirely IRQ driven and is automatically
     * initiated by the stack's startup() call. You need do nothing except configure
     * this class into your stack.
     */

    template<class TTransportLayer>
    class DhcpClient : public virtual TTransportLayer {

      public:

        /**
         * The parameters class
         */

        struct Parameters {

          uint32_t dhcp_responseTimeout;            ///< number of seconds to wait for the UDP packets to get a response (default is 6)
          uint8_t dhcp_retries;                     ///< number of times to retry on failure (default is 5)
          bool dhcp_expectUnicastResponse;          ///< true if the DHCP server should call back via unicast (default is false)
          bool dhcp_requestSubnetMask;              ///< ask for the subnet mask (default is true)
          bool dhcp_requestDefaultGateway;          ///< ask for the default gateway (default is true)
          bool dhcp_requestDnsServer;               ///< ask for the DNS server (default is true)
          bool dhcp_requestDomainName;              ///< ask for the domain name (default is true)
          const char *dhcp_hostname;                ///< we'll tell the DHCP server that this is your name

          /**
           * Constructor, set defaults
           */

          Parameters() {
            dhcp_responseTimeout=6;                 // max of 6 secs to wait for a UDP response
            dhcp_requestSubnetMask=true;            // ask for the subnet mask
            dhcp_requestDefaultGateway=true;        // ask for the default gateway
            dhcp_requestDnsServer=true;             // ask for the DNS server
            dhcp_requestDomainName=true;            // ask for our domain name
            dhcp_expectUnicastResponse=false;       // we want a broadcast response
            dhcp_retries=5;                         // default times to retry
            dhcp_hostname=nullptr;
          }
        };


        /**
         * Error codes
         */

        enum {
          E_UNCONFIGURED,           ///< we don't have a client mac address
          E_UNEXPECTED_STATE,       ///< an internal error: the state machine is bad
          E_NO_DHCP_SERVER_OPTION,  ///< OFFER message does not contain DHCP server
          E_UDP_SEND_FAILED,        ///< failed to send a UDP message
          E_FAILED                  ///< could not complete the process
        };

      protected:

        /**
         * Possible DHCP states
         */

        enum class State {
          DISCOVERY,            ///< ready to discover DHCP servers
          OFFER,                ///< ready to receive an offer
          REQUEST,              ///< ready to send the request
          ACK,                  ///< ready to receive the ACK
          COMPLETED,            ///< completed OK
          FAILED                ///< process failed
        };

        volatile State _state;                          ///< the current state
        Parameters _params;                             ///< The initialisation parameters
        MacAddress _myMacAddress;                       ///< Received via a notification event
        uint32_t _transactionId;                        ///< Current transaction id
        uint32_t _stateBeginTime;                       ///< last start time of an async operation
        uint8_t _retryIndex;                            ///< current retry index
        IpAddress _ipAddress;                           ///< IP address from ACK packet
        IpAddress _dhcpServerAddress;                   ///< DHCP server address from OFFER packet
        IpSubnetMask _subnetMask;                       ///< requested subnet mask
        IpAddress _defaultGateway;                      ///< default gateway address
        IpAddress _dnsServers[3];                       ///< DNS server addresses
        uint32_t _expiryTime;                           ///< when this lease expires
        scoped_array<char> _domainName;                 ///< our domain name

        volatile DhcpPacket::MessageType _expectedResponseMessage;  ///< which message type is expected next
        DhcpPacket volatile *_responsePacket;           ///< the received packet

      protected:
        bool stateIdle();
        bool stateDiscovery();
        bool stateOffer();
        bool stateRequest();
        bool stateAck();
        void freeResponsePacket();
        bool internalDhcpClientAcquire();

        void retryRestart(NetworkIntervalTickData& nitd);
        void handleOfferTick(NetworkIntervalTickData& nitd);
        void handleAckTick(NetworkIntervalTickData& nitd);
        void handleCompleteTick(NetworkIntervalTickData& nitd);

        void onNotification(NetEventDescriptor& ned);
        void onReceive(UdpDatagramEvent& ned);
        void onTick(NetworkIntervalTickData& nitd);

      public:
        DhcpClient();
        ~DhcpClient();

        bool initialise(const Parameters& params);
        bool startup();

        bool dhcpClientAcquire();
    };


    /**
     * Constructor
     */

    template<class TTransportLayer>
    inline DhcpClient<TTransportLayer>::DhcpClient() {
      _responsePacket=nullptr;
    }


    /**
     * Destructor
     */

    template<class TTransportLayer>
    inline DhcpClient<TTransportLayer>::~DhcpClient() {
      freeResponsePacket();
    }


    /**
     * Initialise the class
     * @param params The parameters class
     * @return true always
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::initialise(const Parameters& params) {

      // remember parameters

      _params=params;

      // initialise members

      _transactionId=0;
      _expectedResponseMessage=DhcpPacket::MessageType::NONE;

      // subscribe to notifications and receive events

      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&DhcpClient<TTransportLayer>::onNotification));
      this->UdpReceiveEventSender.insertSubscriber(UdpReceiveEventSourceSlot::bind(this,&DhcpClient<TTransportLayer>::onReceive));

      // start the ticker off as disabled

      this->subscribeIntervalTicks(0,NetworkIntervalTicker::TickIntervalSlotType::bind(this,&DhcpClient<TTransportLayer>::onTick));
      return true;
    }


    /**
     * Startup the class. Broadcast the IP address and subnet mask as an event
     * @return true
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::startup() {

      if(!dhcpClientAcquire())
        return false;

      // adjust our subscription to the RTC ticker to 1 second intervals

      this->updateIntervalTickSubscription(1,NetworkIntervalTicker::TickIntervalSlotType::bind(this,&DhcpClient<TTransportLayer>::onTick));

      // wait for the process to complete or fail

      while(_state!=State::COMPLETED && _state!=State::FAILED);
      return _state==State::COMPLETED;
    }


    /**
     * Go through the process of acquiring what we need
     * @return true if succeeded
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::dhcpClientAcquire() {

      _retryIndex=0;
      return internalDhcpClientAcquire();
    }

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::internalDhcpClientAcquire() {

      // we must have received a mac address announcement from the lower layers

      if(!_myMacAddress.isValid())
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DHCP,E_UNCONFIGURED);

      _state=State::DISCOVERY;
      _transactionId++;
      _ipAddress.invalidate();
      _subnetMask.invalidate();
      _defaultGateway.invalidate();
      _dnsServers[0].invalidate();
      _dnsServers[1].invalidate();
      _dnsServers[2].invalidate();
      _dhcpServerAddress.invalidate();
      _expiryTime=UINT32_MAX;
      _domainName.reset();

      freeResponsePacket();

      // start if off. the remainder of the process happens via IRQ

      return stateDiscovery();
    }


    /**
     * DISCOVERY state handler
     * @return true if successfully advanced to next state
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::stateDiscovery() {

      DhcpPacket *dp;
      char buffer[314];

      // create a DISCOVERY packet on the stack

      memset(buffer,0,sizeof(buffer));
      dp=reinterpret_cast<DhcpPacket *>(&buffer);
      dp->createDiscovery(_myMacAddress,
                           _transactionId,
                           _params.dhcp_expectUnicastResponse,
                           _params.dhcp_requestDefaultGateway,
                           _params.dhcp_requestSubnetMask,
                           _params.dhcp_requestDnsServer,
                           _params.dhcp_requestDomainName);

      // Send the packet asynchronously over UDP. At this point we don't know
      // our subnet mask so the broadcast address is all 1's.

      _expectedResponseMessage=DhcpPacket::MessageType::OFFER;

      if(!this->udpSend("255.255.255.255",                      // broadcast address
                        IpPorts::PORT_DHCP_RESPONSE,            // source port
                        IpPorts::PORT_DHCP_REQUEST,             // destination port,
                        reinterpret_cast<uint8_t *>(dp),        // data
                        sizeof(buffer),                         // data size
                        true,                                   // asynchronous
                        0)) {                                   // time to wait (ignored)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DHCP,E_UDP_SEND_FAILED);
      }

      // advance to the next state

      _stateBeginTime=this->getRtc().getTick();
      _state=State::OFFER;
      return true;
    }


    /**
     * OFFER state handler
     * @return true if successfully advanced to next state
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::stateOffer() {

      // get the server address from the response - it's not volatile now

      uint8_t *options;
      if((options=const_cast<DhcpPacket *>(_responsePacket)->getOption(54))==nullptr)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DHCP,E_NO_DHCP_SERVER_OPTION);

      // get the DHCP server address from the response

      _dhcpServerAddress.ipAddress=*reinterpret_cast<uint32_t *>(options+2);

      // get our offered address

      _ipAddress=const_cast<IpAddress&>(_responsePacket->dhcp_yiaddr);

      // free the response packet

      freeResponsePacket();
      _state=State::REQUEST;

      return true;
    }


    /**
     * REQUEST state handler
     * @return true if successfully advanced to next state
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::stateRequest() {

      DhcpPacket *dp;
      char buffer[314];

      // create a DISCOVERY packet on the stack

      memset(buffer,0,sizeof(buffer));
      dp=reinterpret_cast<DhcpPacket *>(&buffer);
      dp->createRequest(_myMacAddress,
                        _transactionId,
                        _dhcpServerAddress,
                        _ipAddress,
                        _params.dhcp_hostname,
                        _params.dhcp_expectUnicastResponse,
                        _params.dhcp_requestDefaultGateway,
                        _params.dhcp_requestSubnetMask,
                        _params.dhcp_requestDnsServer,
                        _params.dhcp_requestDomainName);

      // Send the packet asynchronously over UDP to the DHCP server

      _expectedResponseMessage=DhcpPacket::MessageType::ACK;

      if(!this->udpSend("255.255.255.255",                      // broadcast address
                        IpPorts::PORT_DHCP_RESPONSE,            // source port
                        IpPorts::PORT_DHCP_REQUEST,             // destination port,
                        reinterpret_cast<uint8_t *>(dp),        // data
                        sizeof(buffer),                         // data size
                        true,                                   // asynchronous
                        0)) {                                   // time to wait (ignored)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DHCP,E_UDP_SEND_FAILED);
      }

      // advance to the next state

      _state=State::ACK;
      _stateBeginTime=this->getRtc().getTick();
      return true;
    }


    /**
     * ACK state handler
     * @return true if successfully advanced to next state
     */

    template<class TTransportLayer>
    inline bool DhcpClient<TTransportLayer>::stateAck() {

      uint32_t lease;

      // get the information from the response - it's no longer volatile

      _ipAddress=const_cast<IpAddress&>(_responsePacket->dhcp_yiaddr);

      uint8_t *options;
      options=reinterpret_cast<uint8_t *>(const_cast<DhcpPacket *>(_responsePacket))+DhcpPacket::getPacketSize();

      while(*options!=255) {

        if(*options==0)             // code 0 is just a null byte
          options++;
        else {
          switch(*options) {

            case 1:                 // subnet mask
              _subnetMask.ipAddress=*reinterpret_cast<uint32_t *>(options+2);
              break;

            case 3:                 // router (default gateway)
              _defaultGateway.ipAddress=*reinterpret_cast<uint32_t *>(options+2);
              break;

            case 6:                 // DNS servers (up to 3)
              if(options[1]>=4)
                _dnsServers[0].ipAddress=*reinterpret_cast<uint32_t *>(options+2);
              if(options[1]>=8)
                _dnsServers[1].ipAddress=*reinterpret_cast<uint32_t *>(options+6);
              if(options[1]>=12)
                _dnsServers[2].ipAddress=*reinterpret_cast<uint32_t *>(options+10);
              break;

            case 15:
              _domainName.reset(new char[options[1]+1]);
              memcpy(_domainName.get(),options+2,options[1]);
              _domainName[options[1]]='\0';
              break;

            case 51:                // lease time in seconds (renew 50% of time through)
              lease=NetUtil::ntohl(*reinterpret_cast<uint32_t *>(options+2));
              _expiryTime=this->_rtc->getTick()+lease;
              break;
          }

          // move to next option

          options+=2+options[1];
        }
      }

      // free the response packet

      freeResponsePacket();

      // announce our information

      if(_ipAddress.isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpAddressAnnouncementEvent(_ipAddress));

      if(_subnetMask.isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpSubnetMaskAnnouncementEvent(_subnetMask));

      if(_defaultGateway.isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpDefaultGatewayAnnouncementEvent(_defaultGateway));

      if(_dnsServers[0].isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpDnsServersAnnouncementEvent(_dnsServers));

      // we are completed

      _state=State::COMPLETED;
      return true;
    }


    /**
     * Free the response packet
     */

    template<class TTransportLayer>
    inline void DhcpClient<TTransportLayer>::freeResponsePacket() {

      if(_responsePacket) {
        free((void *)_responsePacket);
        _responsePacket=nullptr;
        _expectedResponseMessage=DhcpPacket::MessageType::NONE;
      }
    }


    /**
     * Notification event notification from the stack
     * @param ned The network event descriptor
     */

    template<class TTransportLayer>
    __attribute__((noinline)) inline void DhcpClient<TTransportLayer>::onNotification(NetEventDescriptor& ned) {

      // we're interested in mac address announcements

      if(ned.eventType!=NetEventDescriptor::NetEventType::MAC_ADDRESS_ANNOUNCEMENT)
        return;

      _myMacAddress=static_cast<MacAddressAnnouncementEvent&>(ned).macAddress;
    }


    /**
     * Receive event notification from the stack
     * @param upe The UDP packet event
     */

    template<class TTransportLayer>
    __attribute__((noinline)) inline void DhcpClient<TTransportLayer>::onReceive(UdpDatagramEvent& upe) {

      // handle UDP packets when we're waiting

      if(_expectedResponseMessage==DhcpPacket::MessageType::NONE || _responsePacket!=nullptr)
        return;

      // must be a DHCP packet on the response port

      if(NetUtil::ntohs(upe.udpDatagram.udp_destinationPort)!=IpPorts::PORT_DHCP_RESPONSE ||
         NetUtil::ntohs(upe.udpDatagram.udp_length)<UdpDatagram::getHeaderSize())
        return;

      // it's valid for our port

      upe.handled=true;

      // check the packet content magic number

      DhcpPacket *packet=reinterpret_cast<DhcpPacket *>(upe.udpDatagram.udp_data);
      if(packet->dhcp_magic!=NetUtil::htonl(0x63825363))
        return;

      // must be for the current session and our mac address and be a reply

      if(packet->dhcp_xid!=_transactionId || packet->dhcp_chaddr!=_myMacAddress || packet->dhcp_op!=DhcpPacket::Operation::REPLY)
        return;

      // must be the message type we expect

      if(_expectedResponseMessage!=packet->getMessageType())
        return;

      // store the packet and release the waiting code. not too concerned about the performance
      // overhead of copying packets around here because DHCP only runs when the system is
      // starting up or renewing.

      uint16_t packetLength=NetUtil::ntohs(upe.udpDatagram.udp_length);

      DhcpPacket *rp=reinterpret_cast<DhcpPacket *>(malloc(packetLength));
      memcpy(reinterpret_cast<void *>(rp),packet,packetLength);

      _responsePacket=rp;
    }


    /**
     * This IRQ code is a callback from our subscription to the NetworkIntervalTicker
     * class. We use ticks of the RTC to manage state transitions, timeout detection
     * and automatic lease renewal.
     * @param nitd The tick data
     */

    template<class TTransportLayer>
    __attribute__((noinline)) inline void DhcpClient<TTransportLayer>::onTick(NetworkIntervalTickData& nitd) {

      if(_state==State::OFFER)
        handleOfferTick(nitd);
      else if(_state==State::ACK)
        handleAckTick(nitd);
      else if(_state==State::COMPLETED)
        handleCompleteTick(nitd);
    }


    /**
     * The RTC clock ticked while we were in the OFFER state
     */

    template<class TTransportLayer>
    inline void DhcpClient<TTransportLayer>::handleOfferTick(NetworkIntervalTickData& nitd) {

      if(_responsePacket==nullptr) {

        // check for timeout

        if(nitd.timeNow-_stateBeginTime>_params.dhcp_responseTimeout)
          retryRestart(nitd);         // timed out, retry the process
      }
      else {
        if(!stateOffer() || !stateRequest())        // invalid data in the response, retry restart
          retryRestart(nitd);
      }
    }


    /**
     * The RTC clock ticked while we were in the ACK state
     */

    template<class TTransportLayer>
    inline void DhcpClient<TTransportLayer>::handleAckTick(NetworkIntervalTickData& nitd) {

      if(_responsePacket==nullptr) {

        // check for timeout

        if(nitd.timeNow-_stateBeginTime>_params.dhcp_responseTimeout)
          retryRestart(nitd);         // timed out, retry the process
      }
      else {

        // process the ACK

        if(!stateAck())
          retryRestart(nitd);

        // the interval ticker is now updated to come back when we're
        // half way through the lease so we can renew it automatically

        nitd.interval=_expiryTime/2;
      }
    }


    /**
     * Something went wrong during the process. start it off again
     */

    template<class TTransportLayer>
    inline void DhcpClient<TTransportLayer>::retryRestart(NetworkIntervalTickData& nitd) {

      if(++_retryIndex==_params.dhcp_retries) {

        // run out of retries, cancel the timer

        _state=State::FAILED;
        this->setError(ErrorProvider::ERROR_PROVIDER_NET_DHCP,E_FAILED);
        nitd.interval=0;
      }
      else {

        if(internalDhcpClientAcquire())
          nitd.interval=1;                  // we're off again
        else {

          // cannot even restart the process, cancel the timer

          _state=State::FAILED;
          this->setError(ErrorProvider::ERROR_PROVIDER_NET_DHCP,E_FAILED);
          nitd.interval=0;
        }
      }
    }


    /**
     * RTC clock ticked while we were in the COMPLETED state. This can only mean that
     * the lease is to be renewed
     */

    template<class TTransportLayer>
    inline void DhcpClient<TTransportLayer>::handleCompleteTick(NetworkIntervalTickData& nitd) {

      // use the notification event to tell clients

      this->NetworkNotificationEventSender.raiseEvent(DhcpRenewalDueEvent(_expiryTime));

      // start the process again

      dhcpClientAcquire();

      // ticks are now at 1 second intervals

      nitd.interval=1;
    }
  }
}
