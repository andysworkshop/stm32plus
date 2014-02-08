/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a client Mac address announcement
     */

    struct MacAddressAnnouncementEvent : NetEventDescriptor {

      /**
       * References to the value
       */

      const MacAddress& macAddress;


      /**
       * Constructor
       */

      MacAddressAnnouncementEvent(const MacAddress& address)
        : NetEventDescriptor(NetEventType::MAC_ADDRESS_ANNOUNCEMENT),
          macAddress(address) {
      }
    };
  }
}
