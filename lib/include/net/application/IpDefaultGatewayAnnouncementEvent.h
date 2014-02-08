/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Your default gateway (router)
     */

    struct IpDefaultGatewayAnnouncementEvent : NetEventDescriptor {

      /**
       * References to the values
       */

      const IpAddress& defaultGateway;


      /**
       * Constructor
       */

      IpDefaultGatewayAnnouncementEvent(const IpAddress& gateway)
        : NetEventDescriptor(NetEventType::DEFAULT_GATEWAY_ANNOUNCEMENT),
          defaultGateway(gateway) {
      }
    };
  }
}
