/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Application layer feature to provide the functionality of the "Ping" application
     * that sends and receives ICMP packets
     */

    template<class TTransportLayer>
    class Ping : public virtual TTransportLayer {

      public:
        struct Parameters {
          uint32_t ping_timeout;          ///< 1000ms is the default
          const uint8_t *ping_data;       ///< the alphabet is the default if this is null
          uint32_t ping_dataLength;       ///< only matters if ping_data is not null
          uint8_t ping_ttl;               ///< default (0) lets the IP layer use its configured value

          Parameters() {
            ping_timeout=1000;
            ping_data=nullptr;
            ping_ttl=0;
          }
        };

        /**
         * error codes
         */

        enum {
          E_TIMEOUT = 1,                  ///< timed out waiting for a response
          E_ERROR_REPLY                   ///< got a reply, but it was an error
        };

      protected:
        Parameters _params;               ///< stored parameters
        uint16_t _nextId;                 ///< the next identifier to put into a ping packet

        volatile bool _waiting;           ///< reset by the onReceive IRQ when we get a reply or error
        IcmpType _icmpType;             ///< taken from the ICMP reply/error packet header
        IcmpCode _icmpCode;             ///< taken from the ICMP reply/error packet header

        /**
         * struct that we can cast directly on to the ping payload
         */

        struct PingData {
          uint32_t millis;          ///< timestamp
          uint8_t data[1];          ///< variable length data
        } __attribute__((packed));

      protected:
        void onReceive(NetEventDescriptor& ned);

      public:
        bool initialise(const Parameters& params);
        bool startup();

        bool ping(const IpAddress& host,uint32_t& elapsed);

        uint32_t pingGetElapsed() const;
        IcmpType pingGetIcmpType() const;
    };


    /**
     * Initialise the class
     * @param params The parameters
     * @return true if it worked
     */

    template<class TTransportLayer>
    inline bool Ping<TTransportLayer>::initialise(const Parameters& params) {

      static const char *_defaultData="abcdefghijklmnopqrstuvwxyz";

      // remember params

      _params=params;

      if(_params.ping_data==nullptr) {
        _params.ping_data=reinterpret_cast<const uint8_t *>(_defaultData);
        _params.ping_dataLength=26;
      }

      // next id is zero

      _nextId=0;
      _waiting=false;

      // subscribe to receive events from the network

      this->NetworkReceiveEventSender.insertSubscriber(NetworkReceiveEventSourceSlot::bind(this,&Ping<TTransportLayer>::onReceive));
      return true;
    }


    /**
     * Startup the feature
     * @return true
     */

    template<class TTransportLayer>
    inline bool Ping<TTransportLayer>::startup() {
      return true;
    }


    /**
     * Ping the host and wait for a reply - once only
     * @param host The IP address of the host to ping.
     * @param elapsed The elapsed time in ms. for a successful ping.
     * @return true if a reply was received, false otherwise.
     */

    template<class TTransportLayer>
    inline bool Ping<TTransportLayer>::ping(const IpAddress& host,uint32_t& elapsed) {

      uint32_t sent;

      // send the echo request

      _waiting=true;
      elapsed=0;

      sent=MillisecondTimer::millis();
      this->icmpSendEchoRequest(host,_nextId,_params.ping_data,_params.ping_dataLength,_params.ping_ttl);

      // wait for a response or timeout

      while(_waiting) {

        elapsed=MillisecondTimer::millis()-sent;

        if(elapsed>_params.ping_timeout) {

          // timed out, don't reuse this id so soon

          _nextId++;
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_PING,E_TIMEOUT);
        }
      }

      // got a response

      _nextId++;

      // echo reply is a good response

      if(_icmpType==IcmpType::ECHO_REPLY)
        return true;

      // anything else is an error

      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_NET_PING,E_ERROR_REPLY);
    }


    /**
     * Receive notification callback. We're interested in ICMP receive events
     * that we can use to correlate against the messages we send.
     * @param ned The network event descriptor base classes
     */

    template<class TTransportLayer>
    inline void Ping<TTransportLayer>::onReceive(NetEventDescriptor& ned) {

      // only interested in ICMP packets

      if(ned.eventType!=NetEventDescriptor::NetEventType::ICMP_PACKET)
        return;

      // not interested unless we solicited this packet

      if(!_waiting)
        return;

      // get the ICMP event

      IcmpPacketEvent& rxevent(static_cast<IcmpPacketEvent&>(ned));

      // not interested if it's not a possible response to a ping

      IcmpType type=rxevent.icmpPacket.icmp_type;

      if(type!=IcmpType::ECHO_REPLY && type!=IcmpType::DESTINATION_UNREACHABLE)
        return;

      // not interested if the id doesn't match the one we're waiting for
      // reply and error messages have the id in the same header position

      if(static_cast<IcmpEchoReply&>(rxevent.icmpPacket).icmp_sequenceNumber!=NetUtil::htons(_nextId))
        return;

      // extract the type/code and set the found flag to release the caller

      _icmpType=rxevent.icmpPacket.icmp_type;
      _icmpCode=rxevent.icmpPacket.icmp_code;

      _waiting=false;
    }


    /**
     * Get the ICMP type code
     * @return The type code
     */

    template<class TTransportLayer>
    inline IcmpType Ping<TTransportLayer>::pingGetIcmpType() const {
      return _icmpType;
    }
  }
}
