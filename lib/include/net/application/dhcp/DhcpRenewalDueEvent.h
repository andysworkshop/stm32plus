/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor that announces that the DHCP release
     * is due for renewal
     */

    struct DhcpRenewalDueEvent : NetEventDescriptor {

      /**
       * The expiry time expressed as an RTC time in ticks. This is a time, not an offset.
       */

      uint32_t renewalTime;

      DhcpRenewalDueEvent(uint32_t rt)
        : NetEventDescriptor(NetEventType::DHCP_RENEWAL_DUE),
          renewalTime(rt) {
      }
    };
  }
}
