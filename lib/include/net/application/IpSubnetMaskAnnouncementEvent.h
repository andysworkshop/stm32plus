/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor that announces a new subnet mask
     */

    struct IpSubnetMaskAnnouncementEvent : NetEventDescriptor {

      /**
       *  Your subnet mask
       */

      const IpSubnetMask& subnetMask;


      /**
       * Constructor
       */

      IpSubnetMaskAnnouncementEvent(const IpSubnetMask& subnetMask)
        : NetEventDescriptor(NetEventType::SUBNET_MASK_ANNOUNCEMENT),
          subnetMask(subnetMask) {
      }
    };
  }
}
