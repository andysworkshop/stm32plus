/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Application layer feature to provide DNS client functionality
     * for IP name resolution
     */

    template<class TTransportLayer>
    class Dns : public virtual TTransportLayer {

      public:
        struct Parameters {

          uint32_t dns_timeout;               ///< 10000ms is the default. don't go less than 5000 according to RFC 1123
          uint32_t dns_cacheSize;             ///< DNS cache size (default is 20)
          uint8_t dns_retries;                ///< number of times to retry all servers (default is 5)

          Parameters() {
            dns_timeout=10000;
            dns_cacheSize=20;
            dns_retries=5;
          }
        };


        /**
         * error codes
         */

        enum {
          E_TIMEOUT = 1,                      ///< timed out waiting for a response
          E_UNCONFIGURED,                     ///< we haven't been given a DNS server
          E_TIMED_OUT,                        ///< timed out waiting for a response
          E_TRUNCATED_REPLY,                  ///< reply was truncated, which it should not be
          E_SERVER_RECURSION_NOT_SUPPORTED,   ///< the server could not recursively resolve the query
          E_SERVER_ERROR,                     ///< the server returned an error (cause is set)
          E_NO_ANSWERS,                       ///< the server did not return an answer
          E_NO_A_RECORD_IN_ANSWERS,           ///< server did not return an A record in the answers
          E_OUT_OF_MEMORY                     ///< memory allocation failure
        };

      protected:
        Parameters _params;
        uint16_t _queryId;
        uint16_t _replyPort;
        IpAddress _dnsServers[3];
        DnsCache _cache;

        volatile bool _awaitingReply;
        DnsReplyPacket volatile *_replyPacket;

      protected:
        void onNotification(NetEventDescriptor& ned);
        void onReceive(UdpDatagramEvent& ned);
        bool queryServer(const IpAddress& dnsServer,const DnsQueryPacket& packet,uint16_t querySize,IpAddress& ipAddress,uint32_t& ttl);
        bool processQueryResponse(IpAddress& ipAddress,uint32_t& ttl);
        void freeReplyPacket();

      public:
        Dns();
        ~Dns();

        bool initialise(const Parameters& params);
        bool startup();

        bool dnsHostnameQuery(const char *hostname,IpAddress& ipAddress);
    };


    /**
     * Constructor
     */

    template<class TTransportLayer>
    inline Dns<TTransportLayer>::Dns() {

      uint32_t randomNumber;

      _dnsServers[0]=nullptr;
      _replyPacket=nullptr;
      _awaitingReply=false;

      this->nextRandom(randomNumber);
      _queryId=randomNumber;
    }


    /**
     * Destructor
     */

    template<class TTransportLayer>
    inline Dns<TTransportLayer>::~Dns() {
      freeReplyPacket();
    }


    /**
     * Initialise the class
     * @param params The parameters
     * @return true if it worked
     */

    template<class TTransportLayer>
    inline bool Dns<TTransportLayer>::initialise(const Parameters& params) {

      // remember params

      _params=params;

      // create the cache

      if(!_cache.initialise(_params.dns_cacheSize,this->_rtc))
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_OUT_OF_MEMORY);

      // subscribe to notifications and receive events

      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&Dns<TTransportLayer>::onNotification));
      this->UdpReceiveEventSender.insertSubscriber(UdpReceiveEventSourceSlot::bind(this,&Dns<TTransportLayer>::onReceive));
      return true;
    }


    /**
     * Startup the feature
     * @return true
     */

    template<class TTransportLayer>
    inline bool Dns<TTransportLayer>::startup() {
      return true;
    }


    /**
     * Notification event notification from the stack
     * @param ned The network event descriptor
     */

    template<class TTransportLayer>
    __attribute__((noinline)) inline void Dns<TTransportLayer>::onNotification(NetEventDescriptor& ned) {

      uint8_t i;

      // only concerned with DNS servers

      if(ned.eventType!=NetEventDescriptor::NetEventType::DNS_SERVERS_ANNOUNCEMENT)
        return;

      IpDnsServersAnnouncementEvent& dnsevent(static_cast<IpDnsServersAnnouncementEvent&>(ned));

      // copy out the valid servers from the event

      for(i=0;i<3;i++)
        if(dnsevent.ipDnsServers[i]!=nullptr && dnsevent.ipDnsServers[i].isValid())
          _dnsServers[i]=*dnsevent.ipDnsServers;

      // invalidate the others

      while(i!=3)
        _dnsServers[i++].invalidate();
    }


    /**
     * This is the basic host namet to IP address lookup functionality
     * @param hostname The host to lookup
     * @param[out] ipAddress The IP address of the host
     * @return true if it worked
     */

    template<class TTransportLayer>
    inline bool Dns<TTransportLayer>::dnsHostnameQuery(const char *hostname,IpAddress& ipAddress) {

      uint16_t querySize;
      uint32_t ttl;
      uint8_t i,retry;

      // special case for localhost

      if(!strcasecmp(hostname,"localhost")) {
        ipAddress.setLocalhost();
        return true;
      }

      // is the association cached?

      if(_cache.lookup(hostname,ipAddress))
        return true;

      // must have at least one server

      if(_dnsServers[0]==nullptr)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_UNCONFIGURED);

      // the size of the query is the hostname+2+header+4

      querySize=DnsPacketHeader::getPacketHeaderSize()+
                strlen(hostname)+2+
                4;

      // allocate the query

      scoped_array<uint8_t> packetData(new uint8_t[querySize]);
      if(packetData.get()==nullptr)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_OUT_OF_MEMORY);

      // create the header and data

      DnsQueryPacket& packet=reinterpret_cast<DnsQueryPacket&>(*(packetData.get()));

      freeReplyPacket();
      _queryId++;
      packet.createQuery(hostname,_queryId);

      // retry for the configured number of times

      for(retry=0;retry<_params.dns_retries;retry++) {

        // run through the list of servers

        for(i=0;i<3 && _dnsServers[i].isValid();i++) {
          if(queryServer(_dnsServers[i],packet,querySize,ipAddress,ttl)) {
            _cache.add(hostname,ipAddress,ttl);
            return true;
          }
        }
      }

      // they all failed, preserve error returned by the last one

      return false;
    }


    /**
     * Query the server and wait for a response
     * @param packet The packet to send
     * @param querySize The size of the packet
     * @param[out] ipAddress The returned IP address mapping
     * @param[out] ttl The time-to-live of the entry in seconds
     * @return true if it worked
     */

    template<class TTransportLayer>
    inline bool Dns<TTransportLayer>::queryServer(const IpAddress& dnsServer,
                                                   const DnsQueryPacket& packet,
                                                   uint16_t querySize,
                                                   IpAddress& ipAddress,
                                                   uint32_t& ttl) {

      uint32_t now;
      bool retval;

      // claim an ephemeral port for the reply

      if(!this->ip_acquireEphemeralPort(_replyPort))
        return false;

      // send the packet asynchronously over UDP to the DHCP server

      _awaitingReply=true;

      if(!this->udpSend(dnsServer,                                  // server to send to
                        _replyPort,                                 // source port
                        IpPorts::PORT_DNS_REQUEST,                  // destination port,
                        reinterpret_cast<const uint8_t *>(&packet), // data
                        querySize,                                  // data size
                        true,                                       // asynchronous flag
                        0)) {                                       // n/a for async
        this->ip_releaseEphemeralPort(_replyPort);
        return false;         // should not happen, async always returns true
      }

      // await a response

      now=MillisecondTimer::millis();
      while(_awaitingReply) {
        if(MillisecondTimer::hasTimedOut(now,_params.dns_timeout)) {
          this->ip_releaseEphemeralPort(_replyPort);
          return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_TIMED_OUT);
        }
      }

      retval=processQueryResponse(ipAddress,ttl);

      freeReplyPacket();
      this->ip_releaseEphemeralPort(_replyPort);
      return retval;
    }


    /**
     * Process the response from the server
     * @param[out] ipAddress The response IP address
     * @param[out] ttl The time-to-live of the entry in seconds
     * @return true if it worked
     */

    template<class TTransportLayer>
    inline bool Dns<TTransportLayer>::processQueryResponse(IpAddress& ipAddress,uint32_t& ttl) {

      uint16_t flags=NetUtil::ntohs(_replyPacket->dns_flags);
      uint8_t *answers,*record;

      // the flags must not have an error (unknown host is picked up here as cause=3)

      if((flags & 0xf)!=0)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_SERVER_ERROR,flags & 0xf);

      // the flags must not indicate truncation

      if((flags & DnsPacketHeader::TRUNCATED)!=0)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_TRUNCATED_REPLY);

      // there must be at least one answer

      if(NetUtil::ntohs(_replyPacket->dns_numberOfAnswerRrs)==0)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_NO_ANSWERS);

      // find the answers

      DnsReplyPacket *packet(const_cast<DnsReplyPacket *>(_replyPacket));
      answers=packet->findAnswers();

      // find the first A record

      if((record=packet->findRecord(answers,DnsPacketHeader::RecordType::A))==nullptr)
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_DNS,E_NO_A_RECORD_IN_ANSWERS);

      // step over the name (should possibly verify that it's the answer to our question here)

      record=packet->stepOverName(record);

      // pull out the IP address and ttl from the record

      ipAddress.ipAddress=*reinterpret_cast<uint32_t *>(record+10);
      ttl=NetUtil::ntohl(*reinterpret_cast<uint32_t *>(record+4));

      return true;
    }


    /**
     * Receive event notification from the stack
     * @param upe The UDP packet event descriptor
     */

    template<class TTransportLayer>
    __attribute__((noinline)) inline void Dns<TTransportLayer>::onReceive(UdpDatagramEvent& upe) {

      // must be for our reply port

      if(NetUtil::ntohs(upe.udpDatagram.udp_destinationPort)!=_replyPort)
        return;

      // it's considered handled now

      upe.handled=true;

      // handle UDP packets when we're waiting

      if(!_awaitingReply || _replyPacket!=nullptr)
        return;

      // must be a valid packet on the DNS port

      if(NetUtil::ntohs(upe.udpDatagram.udp_sourcePort)!=IpPorts::PORT_DNS_REQUEST ||
         NetUtil::ntohs(upe.udpDatagram.udp_length)<DnsPacketHeader::getPacketHeaderSize())
        return;

      DnsQueryPacket *reply(reinterpret_cast<DnsQueryPacket *>(upe.udpDatagram.udp_data));

      // must be our DNS packet. there is no fixed magic number to indicate that this packet is
      // a DNS response. we only have our ID to go on

      if(NetUtil::ntohs(reply->dns_identification)!=_queryId)
        return;

      // it's for us

      _replyPacket=(DnsReplyPacket *)malloc(upe.udpDatagram.udp_length);
      memcpy(const_cast<DnsReplyPacket *>(_replyPacket),upe.udpDatagram.udp_data,upe.udpDatagram.udp_length);

      // wake up the caller

      _awaitingReply=false;
    }


    /**
     * Free the reply packet
     */

    template<class TTransportLayer>
    inline void Dns<TTransportLayer>::freeReplyPacket() {

      if(_replyPacket) {
        free((void *)_replyPacket);
        _replyPacket=nullptr;
        _awaitingReply=false;
      }
    }
  }
}
