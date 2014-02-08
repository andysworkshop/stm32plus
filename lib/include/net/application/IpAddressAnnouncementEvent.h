/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor that announces a new client IP address
     */

    struct IpAddressAnnouncementEvent : NetEventDescriptor {

      /**
       * References to the value
       */

      const IpAddress& ipAddress;           ///< Your IP address


      /**
       * Constructor
       */

      IpAddressAnnouncementEvent(const IpAddress& address)
        : NetEventDescriptor(NetEventType::IP_ADDRESS_ANNOUNCEMENT),
          ipAddress(address) {
      }
    };
  }
}
