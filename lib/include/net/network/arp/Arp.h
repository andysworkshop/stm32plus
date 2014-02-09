/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    DECLARE_EVENT_SIGNATURE(ArpReceive,void (ArpReceiveEvent&));

    /**
     * Network layer feature that implements the Address Resolution Protocol (ARP)
     * ARP is used as to convert one address form into another. In our implementation
     * we will support converting MAC addresses into IP addresses.
     */

    template<class TDatalinkLayer>
    class Arp : public virtual TDatalinkLayer {

      public:

      /**
       * Error codes
       */

        enum {
          E_INVALID_OPERATION = 1,      ///< not a request or a reply
          E_CANNOT_REPLY,               ///< E_CANNOT_REPLY
          E_UNCONFIGURED,               ///< Our MAC/IP are not configured
          E_REQUEST_NOT_PERMITTED,      ///< not permitted to do a request due to IRQ context
          E_IP_ADDRESS_CLASH,           ///< Another station on the net has our IP address
          E_TIMED_OUT                   ///< timed out waiting for a response
        };

        /**
         * Parameters class
         */

        struct Parameters {

          bool arp_startupBroadcast;        ///< true if we broadcast our own address on startup, default is true
          uint16_t arp_cacheSize;           ///< number of entries to include in the ARP cache, default is 10
          uint32_t arp_cacheExpirySeconds;  ///< The max seconds to keep an ARP cache entry, default is 600
          uint32_t arp_replyTimeout;        ///< how long in ms to wait for an ARP reply, default is 5000
          uint8_t arp_retries;              ///< number of times to retry, default is 5

          Parameters() {
            arp_startupBroadcast=true;
            arp_cacheSize=10;               ///< 10 bytes/entry, 10 entries = 100 bytes.
            arp_replyTimeout=5000;          ///< 5 seconds for an ARP timeout
            arp_cacheExpirySeconds=600;     ///< 10 minute default cache lifetime
            arp_retries=5;                  ///< 5 times to retry
          }
        };

        DECLARE_EVENT_SOURCE(ArpReceive);

      protected:
        Parameters _params;
        IpAddress _myIpAddress;
        IpAddress _defaultGatewayAddress;
        IpSubnetMask _mySubnetMask;
        MacAddress _myMacAddress;
        ArpCache _arpCache;

      protected:
        void handleIpAddressAnnouncement(IpAddressAnnouncementEvent& event);
        void handleMacAddressAnnouncement(MacAddressAnnouncementEvent& event);
        void handleSubnetMaskAnnouncement(IpSubnetMaskAnnouncementEvent& event);
        void handleDefaultGatewayAnnouncement(IpDefaultGatewayAnnouncementEvent& event);
        void handleNewAddressMapping(IpAddressMappingEvent& event);
        bool handleAddressMappingRequest(ArpMappingRequestEvent& event);
        bool handleIncomingFrame(const DatalinkFrame& frame);

        void onReceive(NetEventDescriptor& ned);
        void onNotification(NetEventDescriptor& ned);

      public:
        bool initialise(const Parameters&);
        bool startup();

        void arpBroadcastMyAddress();
        void arpSendRequest(IpAddress& ipaddress);
        void arpSendProbe(IpAddress& ipaddress);
    };


    /**
     * Initialise the class
     * @param params The parameters class
     * @return always true
     */

    template<class TDatalinkLayer>
    inline bool Arp<TDatalinkLayer>::initialise(const Parameters& params) {

      // save the parameters

      _params=params;

      // initialise the cache

      _arpCache.initialise(params.arp_cacheSize,params.arp_cacheExpirySeconds,this->_rtc);

      // subscribe to receive notification and receive events

      this->NetworkReceiveEventSender.insertSubscriber(NetworkReceiveEventSourceSlot::bind(this,&Arp<TDatalinkLayer>::onReceive));
      this->NetworkNotificationEventSender.insertSubscriber(NetworkNotificationEventSourceSlot::bind(this,&Arp<TDatalinkLayer>::onNotification));

      return true;
    }


    /**
     * Startup the feature
     * @return
     */

    template<class TDatalinkLayer>
    inline bool Arp<TDatalinkLayer>::startup() {
      return true;
    }


    /**
     * Notification from our subscription to receive events
     * @param ned The event descriptor
     */

    template<class TDatalinkLayer>
    __attribute__((noinline)) inline void Arp<TDatalinkLayer>::onReceive(NetEventDescriptor& ned) {

      // check if it's a datalink frame event

      if(ned.eventType==NetEventDescriptor::NetEventType::DATALINK_FRAME) {

        const DatalinkFrame& frame=static_cast<DatalinkFrameEvent&>(ned).datalinkFrame;

        if(frame.protocol==static_cast<uint16_t>(EtherType::ARP))
          handleIncomingFrame(frame);
      }
    }


    /**
     * Notification from our subscription to notification events
     * @param ned The event descriptor
     */

    template<class TDatalinkLayer>
    __attribute__((noinline)) inline void Arp<TDatalinkLayer>::onNotification(NetEventDescriptor& ned) {

      // if it's a new IP address/mask then we are interested

      if(ned.eventType==NetEventDescriptor::NetEventType::ARP_MAPPING_REQUEST)
        handleAddressMappingRequest(static_cast<ArpMappingRequestEvent&>(ned));

      else if(ned.eventType==NetEventDescriptor::NetEventType::IP_ADDRESS_ANNOUNCEMENT)
        handleIpAddressAnnouncement(static_cast<IpAddressAnnouncementEvent&>(ned));

      else if(ned.eventType==NetEventDescriptor::NetEventType::SUBNET_MASK_ANNOUNCEMENT)
        handleSubnetMaskAnnouncement(static_cast<IpSubnetMaskAnnouncementEvent&>(ned));

      else if(ned.eventType==NetEventDescriptor::NetEventType::DEFAULT_GATEWAY_ANNOUNCEMENT)
        handleDefaultGatewayAnnouncement(static_cast<IpDefaultGatewayAnnouncementEvent&>(ned));

      else if(ned.eventType==NetEventDescriptor::NetEventType::MAC_ADDRESS_ANNOUNCEMENT)
        handleMacAddressAnnouncement(static_cast<MacAddressAnnouncementEvent&>(ned));

      else if(ned.eventType==NetEventDescriptor::NetEventType::IP_ADDRESS_MAPPING)
        handleNewAddressMapping(static_cast<IpAddressMappingEvent&>(ned));
    }


    /**
     * Handle an incoming frame event. It's already confirmed to be ARP. This is IRQ code.
     * @param frame the incoming frame
     */

    template<class TDatalinkLayer>
    inline bool Arp<TDatalinkLayer>::handleIncomingFrame(const DatalinkFrame& frame) {

      ArpFrameData *afd,*reply;
      NetBuffer *nb;

      afd=reinterpret_cast<ArpFrameData *>(frame.payload);

      // we respond to ARP requests (code 1)

      if(afd->arp_operation==ArpOperation::REQUEST) {

        // are they asking for our MAC address?

        if(_myIpAddress!=afd->arp_targetProtocolAddress)
          return true;      // not an error

        // we have to drop the request if nobody has told us what our address and IP are

        if(!_myMacAddress.isValid() || !_myIpAddress.isValid())
          return this->setError(ErrorProvider::ERROR_PROVIDER_NET_ARP,E_UNCONFIGURED);

        // create a net buffer for the response
        // ARP responses are generated automatically in response to an IRQ

        nb=new NetBuffer(
            this->getDatalinkTransmitHeaderSize(),
            sizeof(ArpFrameData),
            nullptr);

        // the ARP frame data is the last in the buffer, and therefore the first
        // to be written to

        reply=reinterpret_cast<ArpFrameData *>(nb->moveWritePointerBack(sizeof(ArpFrameData)));
        reply->initialise();
        reply->createReply(*afd,_myMacAddress,_myIpAddress);

        // raise a transmit event for the datalink layer to pick up

        this->NetworkSendEventSender.raiseEvent(
              EthernetTransmitRequestEvent(nb,reply->arp_targetHardwareAddress,EtherType::ARP,DatalinkChecksum::IP_HEADER_AND_PROTOCOL));
      }
      else if(afd->arp_operation==ArpOperation::REPLY) {

        // it's a reply, verify that someone is not claiming to own our address

        if(afd->arp_senderProtocolAddress==_myIpAddress)
          return this->setError(ErrorProvider::ERROR_PROVIDER_NET_ARP,E_IP_ADDRESS_CLASH);
        else
          _arpCache.insert(afd->arp_senderHardwareAddress,afd->arp_senderProtocolAddress);
      }

      // send an event to anyone that's interested

      ArpReceiveEventSender.raiseEvent(ArpReceiveEvent(*afd));
      return true;
    }


    /**
     * Handle a IP address announcement
     * @param event The event structure
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::handleIpAddressAnnouncement(IpAddressAnnouncementEvent& event) {

      // remember the address

      _myIpAddress=event.ipAddress;

      // are we configured to broadcast on startup?

      if(_params.arp_startupBroadcast)
        arpBroadcastMyAddress();
    }


    /**
     * Handle a subnet mask announcement
     * @param event The event structure
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::handleSubnetMaskAnnouncement(IpSubnetMaskAnnouncementEvent& event) {

      // remember the mask

      _mySubnetMask=event.subnetMask;
    }


    /**
     * Handle a default gateway announcement
     * @param event The event structure
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::handleDefaultGatewayAnnouncement(IpDefaultGatewayAnnouncementEvent& event) {

      // remember the mask

      _defaultGatewayAddress=event.defaultGateway;
    }


    /**
     * Handle a new client MAC address event
     * @param event The event structure
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::handleMacAddressAnnouncement(MacAddressAnnouncementEvent& event) {

      // remember the address

      _myMacAddress=event.macAddress;

      // are we configured to broadcast on startup?

      if(_params.arp_startupBroadcast)
        arpBroadcastMyAddress();
    }


    /**
     * Handle an ARP mapping request. If the requested IP address is covered by the subnet mask
     * then that's the MAC address we'll be looking for, otherwise we'll be trying to find the
     * MAC address of the default gateway.
     * @param event The event that describes the request
     */

    template<class TDatalinkLayer>
    inline bool Arp<TDatalinkLayer>::handleAddressMappingRequest(ArpMappingRequestEvent& event) {

      IpAddress ip;
      uint8_t retry;

      // can handle requests for 255.255.255.255 with no configuration information

      if(event.ipAddress.isBroadcast()) {
        event.macAddress=MacAddress::createBroadcastAddress();
        event.found=true;
        return true;
      }

      // we can handle requests for a multicast MAC with no configuration information
      // this gives us level 1 (tx only) conformance with RFC1112

      if(event.ipAddress.isMulticastGroup()) {
        event.macAddress.createMulticastAddress(event.ipAddress.ipAddressBytes);
        event.found=true;
        return true;
      }

      // must be configured with a subnet mask and gateway

      if(!_mySubnetMask.isValid() || !_defaultGatewayAddress.isValid())
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_ARP,E_UNCONFIGURED);

      // use the event IP address or the gateway as the lookup

      if(_mySubnetMask.matches(event.ipAddress,_defaultGatewayAddress)) {

        // is the IP address the broadcast address for this subnet?

        if(_mySubnetMask.isBroadcastAddress(event.ipAddress)) {
          event.macAddress=MacAddress::createBroadcastAddress();
          event.found=true;
          return true;
        }
        else
          ip=event.ipAddress;           // a local IP address
      }
      else
        ip=_defaultGatewayAddress;      // a remote IP address

      // first check the cache and return if found or we cannot do a lookup

      if((event.found=_arpCache.findMacAddress(ip,event.macAddress)))
        return true;

      // if we're in an IRQ then we can't continue because we'd need to do a tx/rx

      if(Nvic::isAnyIrqActive())
        return this->setError(ErrorProvider::ERROR_PROVIDER_NET_ARP,E_REQUEST_NOT_PERMITTED);

      for(retry=0;retry<_params.arp_retries;retry++) {

        _arpCache.setWatchIp(ip,&event.macAddress);

        // send off the query

        arpSendRequest(ip);

        // wait for our watcher to be triggered, or a timeout

        if((event.found=_arpCache.waitForWatch(_params.arp_replyTimeout)))
          return true;
      }

      return this->setError(ErrorProvider::ERROR_PROVIDER_NET_ARP,E_TIMED_OUT);
    }


    /**
     * Send an ARP request.
     * @param ipaddress the IP address to include in the query
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::arpSendRequest(IpAddress& ipaddress) {

      NetBuffer *nb;
      ArpFrameData *afd;

      // create the packet. we need space for the datalink
      // layer tx header size plus the size of an ARP frame

      nb=new NetBuffer(
          this->getDatalinkTransmitHeaderSize(),
          sizeof(ArpFrameData),
          nullptr);

      // the ARP frame data is the last in the buffer, and therefore the first
      // to be written to

      afd=reinterpret_cast<ArpFrameData *>(nb->moveWritePointerBack(sizeof(ArpFrameData)));
      afd->initialise();
      afd->createRequest(ipaddress,_myMacAddress,_myIpAddress);

      // raise a transmit event for the datalink layer to pick up

      this->NetworkSendEventSender.raiseEvent(
            EthernetTransmitRequestEvent(nb,MacAddress::createBroadcastAddress(),EtherType::ARP,DatalinkChecksum::IP_HEADER_AND_PROTOCOL));
    }


    /**
     * Send an ARP probe. Sender IP = 0, target IP = ipaddress, sender mac=mymac, target mac=null
     * @param ipaddress the IP address to probe for
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::arpSendProbe(IpAddress& ipaddress) {

      NetBuffer *nb;
      ArpFrameData *afd;

      // create the packet. we need space for the datalink
      // layer tx header size plus the size of an ARP frame

      nb=new NetBuffer(
          this->getDatalinkTransmitHeaderSize(),
          sizeof(ArpFrameData),
          nullptr);

      // the ARP frame data is the last in the buffer, and therefore the first
      // to be written to

      afd=reinterpret_cast<ArpFrameData *>(nb->moveWritePointerBack(sizeof(ArpFrameData)));
      afd->initialise();
      afd->createRequest(ipaddress,_myMacAddress,IpAddress());

      // raise a transmit event for the datalink layer to pick up

      this->NetworkSendEventSender.raiseEvent(
            EthernetTransmitRequestEvent(nb,MacAddress::createBroadcastAddress(),EtherType::ARP,DatalinkChecksum::IP_HEADER_AND_PROTOCOL));
    }


    /**
     * Handle a new address mapping event (MAC <-> association)
     * @param event The address mapping event
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::handleNewAddressMapping(IpAddressMappingEvent& event) {

      if(_mySubnetMask.isValid() && _defaultGatewayAddress.isValid()) {

        // insert the association 'as reported' if the host is on this subnet
        // otherwise the association is between the default gateway and this IP address

        if(_mySubnetMask.matches(event.ipAddress,_defaultGatewayAddress))
          _arpCache.insert(event.macAddress,event.ipAddress);
        else
          _arpCache.insert(event.macAddress,_defaultGatewayAddress);
      }
    }


    /**
     * Broadcast my IP address/MAC address to all and sundry
     */

    template<class TDatalinkLayer>
    inline void Arp<TDatalinkLayer>::arpBroadcastMyAddress() {

      ArpFrameData *afd;
      NetBuffer *nb;

      // can only broadcast if we have a mac and ip

      if(!_myMacAddress.isValid() || !_myIpAddress.isValid())
        return;

      // create the broadcast packet. we need space for the datalink
      // layer tx header size plus the size of an ARP frame

      nb=new NetBuffer(
          this->getDatalinkTransmitHeaderSize(),
          sizeof(ArpFrameData),
          nullptr);

      // the ARP frame data is the last in the buffer, and therefore the first
      // to be written to

      afd=reinterpret_cast<ArpFrameData *>(nb->moveWritePointerBack(sizeof(ArpFrameData)));
      afd->initialise();
      afd->createGratuitous(_myMacAddress,_myIpAddress);

      // raise a transmit event for the datalink layer to pick up

      this->NetworkSendEventSender.raiseEvent(
            EthernetTransmitRequestEvent(nb,MacAddress::createBroadcastAddress(),EtherType::ARP,DatalinkChecksum::IP_HEADER_AND_PROTOCOL));
    }
  }
}
