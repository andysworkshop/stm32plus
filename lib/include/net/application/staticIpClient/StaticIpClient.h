/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Application layer feature to provide a static IP address and subnet mask.
     * This is an alternative to using a DHCP server
     */

    template<class TTransportLayer>
    class StaticIpClient : public virtual TTransportLayer {

      public:

        struct Parameters {
          IpAddress staticip_address;
          IpSubnetMask staticip_subnetMask;
          IpAddress staticip_defaultGateway;
          IpAddress staticip_dnsServers[3];
          const char *staticip_domainName;

          Parameters()
            : staticip_domainName(nullptr) {
          }
        };

      protected:
        Parameters _params;

      public:
        bool initialise(const Parameters& params);
        bool startup();
    };


    /**
     * Initialise the class
     * @param params The parameters class containing the IP address and subnet mask
     * @return true always
     */

    template<class TTransportLayer>
    inline bool StaticIpClient<TTransportLayer>::initialise(const Parameters& params) {
      _params=params;
      return true;
    }


    /**
     * Startup the class. Broadcast the IP address and subnet mask as an event
     * @return true
     */

    template<class TTransportLayer>
    inline bool StaticIpClient<TTransportLayer>::startup() {

      // send the events for anything that we've been configured with

      if(_params.staticip_address.isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpAddressAnnouncementEvent(_params.staticip_address));

      if(_params.staticip_subnetMask.isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpSubnetMaskAnnouncementEvent(_params.staticip_subnetMask));

      if(_params.staticip_defaultGateway.isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpDefaultGatewayAnnouncementEvent(_params.staticip_defaultGateway));

      if(_params.staticip_dnsServers[0].isValid())
        this->NetworkNotificationEventSender.raiseEvent(IpDnsServersAnnouncementEvent(_params.staticip_dnsServers));

      if(_params.staticip_domainName)
        this->NetworkNotificationEventSender.raiseEvent(DomainNameAnnouncementEvent(_params.staticip_domainName));

      // done

      return true;
    }
  }
}
