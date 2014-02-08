/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * This link-local IP client provides the means to obtain an address from the 169.254/16
     * range that does not conflict with any other stations on this ARP domain. ARP must also
     * be configured into your network stack. The ability to start the search from a random
     * address or from a pre-defined address is provided. Starting from a pre-defined address
     * gives some level of predictability to the address that you will be assigned.
     *
     * This client implements RFC3927 within the constraints of this environment. There are
     * some differences, particularly in the selection and duration of time intervals, however
     * the general procedure is complete.
     */

    template<class TTransportLayer>
    class LinkLocalIp : public virtual TTransportLayer {

      public:

        /**
         * Error codes
         */

        enum {
          E_UNCONFIGURED = 1,         ///< The MAC address has not been notified to us by the link layer
          E_NO_ADDRESSES_FOUND,       ///< could not find an address within the retry parameters
          E_ADDRESS_CLASH             ///< there was an address clash during operation
        };

      public:

        /**
         * Parameters class
         */

        struct Parameters {

          bool llip_randomStart;            ///< true to start search from random address, false to use the fixed address as a start. default is true.
          IpAddress llip_fixedStart;      ///< if llip_randomStart is false then this is where the search starts
          uint16_t llip_tries;              ///< total number of tries for different addresses. Default is 30 (about 180 seconds)

          /**
           * Constructor
           */

          Parameters() {
            llip_randomStart=true;
            llip_tries=30;
          }
        };

      protected:

        /**
         * Possible states
         */

        enum class State : uint8_t {
          NONE,                       ///< just initialised
          ERROR,                      ///< unable to get an address
          PROBING,                    ///< we are probing, _probeNumber is the number of this probe
          ANNOUNCING,                 ///< we're sending 2 ARP announcements
          DEFENDING                   ///< we are idling, waiting for trouble.
        };

        volatile State _state;
        volatile IpAddress _currentProbe;
        volatile uint8_t _probeNumber;
        volatile uint8_t _announceNumber;
        volatile bool _addressInUse;

        Parameters _params;
        MacAddress _myMacAddress;
        uint16_t _tryNumber;

      protected:
        bool startProbing();

        void getRandomAddress();
        void onTick(NetworkIntervalTickData& nitd);
        void onNotification(NetEventDescriptor& ned);
        void onReceive(ArpReceiveEvent& are);

      public:
        bool initialise(const Parameters& params);
        bool startup();
    };


    /**
     * Initialise the class
     * @param params The parameters
     * @return true always
     */

    template<class TTransportLayer>
    inline bool LinkLocalIp<TTransportLayer>::initialise(const Parameters& params) {

      // remember params

      _params=params;

      // idle for now

      _state=State::NONE;

      // subscribe to ARP messages

      this->ArpReceiveEventSender.insertSubscriber(ArpReceiveEventSourceSlot::bind(this,&LinkLocalIp<TTransportLayer>::onReceive));

      // subscribe to notifications (to get the stack's MAC)

      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&LinkLocalIp<TTransportLayer>::onNotification));

      // set up our RTC ticker, initially disabled

      this->subscribeIntervalTicks(0,NetworkIntervalTicker::TickIntervalSlotType::bind(this,&LinkLocalIp<TTransportLayer>::onTick));
      return true;
    }


    /**
     * Startup the class: start the sequence
     */

    template<class TTransportLayer>
    inline bool LinkLocalIp<TTransportLayer>::startup() {

      State s;

      // start probing

      if(!startProbing())
        return false;

      // wait for error or DEFENDING state

      while((s=_state)!=State::ERROR && s!=State::DEFENDING);
      return s==State::DEFENDING;
    }


    /**
     * Start probing
     */

    template<class TTransportLayer>
    inline bool LinkLocalIp<TTransportLayer>::startProbing() {

      // set the current state

      _probeNumber=0;
      _tryNumber=0;
      _state=State::PROBING;

      // must have a MAC address

      if(!_myMacAddress.isValid())
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_LINK_LOCAL_IP,E_UNCONFIGURED);

      // set the current probe address

      if(_params.llip_randomStart)
        getRandomAddress();
      else
        const_cast<IpAddress&>(_currentProbe)=_params.llip_fixedStart;

      // send the ARP probe request (source ip = 0, source mac = mymac)

      _addressInUse=false;
      this->arpSendProbe(const_cast<IpAddress&>(_currentProbe));

      // subscribe to a 2 second tick from the RTC

      this->updateIntervalTickSubscription(2,NetworkIntervalTicker::TickIntervalSlotType::bind(this,&LinkLocalIp<TTransportLayer>::onTick));
      return true;
    }


    /**
     * Tick callback from the RTC
     * @param nitd The interval ticker, which we may modify
     */

    template<class TTransportLayer>
    __attribute__((noinline)) inline void LinkLocalIp<TTransportLayer>::onTick(NetworkIntervalTickData& nitd) {

      if(_state==State::PROBING) {

        if(!_addressInUse) {

          if(_probeNumber==2) {

            // announce the address and mask internally to the stack

            this->NetworkNotificationEventSender.raiseEvent(IpAddressAnnouncementEvent(const_cast<IpAddress&>(_currentProbe)));
            this->NetworkNotificationEventSender.raiseEvent(IpSubnetMaskAnnouncementEvent(IpSubnetMask("255.255.0.0")));

            // we're now announcing our address externally using ARP broadcast

            _announceNumber=0;
            _state=State::ANNOUNCING;

            return;
          }

          // probe again

          _probeNumber++;
          this->arpSendProbe(const_cast<IpAddress&>(_currentProbe));

          return;
        }

        // the address is in use
        // have we run out of tries

        if(++_tryNumber==_params.llip_tries) {

          // unsubscribe from the ticker

          nitd.interval=0;

          // set the error and return;

          _state=State::ERROR;
          this->setError(ErrorProvider::ERROR_PROVIDER_NET_LINK_LOCAL_IP,E_NO_ADDRESSES_FOUND);

          return;
        }

        // we have not run out of tries, get a new address and start again

        getRandomAddress();
        _probeNumber=0;
        _addressInUse=false;

        // send the probe

        this->arpSendProbe(const_cast<IpAddress&>(_currentProbe));
      }
      else if(_state==State::ANNOUNCING) {

        // broadcast the announcement after a delay
        // there are 2 announcements

        if(_announceNumber>0)
          this->arpBroadcastMyAddress();

        if(_announceNumber==2) {

          // cancel the timer and move to defending state

          nitd.interval=0;
          _state=State::DEFENDING;
        }
        else
          _announceNumber++;
      }
    }


    /**
     * Subscription to the ARP events
     * @param are The ARP receive event
     */

    template<class TTransportLayer>
    __attribute__((noinline)) void LinkLocalIp<TTransportLayer>::onReceive(ArpReceiveEvent& are) {

      if(_state==State::PROBING) {

        // any ARP reply that has the address we're looking for indicates fail

        if(are.arpFrameData.arp_senderProtocolAddress==const_cast<IpAddress&>(_currentProbe))
          _addressInUse=true;
      }
      else if(_state==State::DEFENDING) {

        // if any ARP frame arrives with our IP address in the sender address and it's not
        // our MAC in the sender mac then we have to get a new address. any open TCP connections
        // will detect the new address assignment and fail.

        if(are.arpFrameData.arp_senderProtocolAddress==const_cast<IpAddress&>(_currentProbe) &&
           are.arpFrameData.arp_senderHardwareAddress!=_myMacAddress) {

          this->setError(ErrorProvider::ERROR_PROVIDER_NET_LINK_LOCAL_IP,E_ADDRESS_CLASH);
          startProbing();
        }
      }
    }


    /**
     * Subscription to the network notification events
     * @param ned The network event descriptor
     */

    template<class TTransportLayer>
    __attribute__((noinline)) void LinkLocalIp<TTransportLayer>::onNotification(NetEventDescriptor& ned) {

      // remember our MAC address when it's notified to us by the link layer

      if(ned.eventType==NetEventDescriptor::NetEventType::MAC_ADDRESS_ANNOUNCEMENT)
        _myMacAddress=static_cast<MacAddressAnnouncementEvent&>(ned).macAddress;
    }


    /**
     * Get a random address for the search. The address space is 169.254.x.y excluding the
     * first and last 256 addresses.
     */

    template<class TTransportLayer>
    inline void LinkLocalIp<TTransportLayer>::getRandomAddress() {

      uint16_t xy;
      uint32_t randomNumber;

      // get a random number from the RNG

      this->nextRandom(randomNumber);
      xy=256+(randomNumber % (65536-512));

      // create the address

      _currentProbe.ipAddressBytes[0]=169;
      _currentProbe.ipAddressBytes[1]=254;
      _currentProbe.ipAddressBytes[2]=xy;
      _currentProbe.ipAddressBytes[3]=xy >> 8;
    }
  }
}
