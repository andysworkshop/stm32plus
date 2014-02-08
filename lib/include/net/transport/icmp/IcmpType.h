/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * ICMP type field
     */

    enum class IcmpType : uint8_t {

      ECHO_REPLY              = 0,
      DESTINATION_UNREACHABLE = 3,
      SOURCE_QUENCH           = 4,
      REDIRECT_MESSAGE        = 5,
      ECHO_REQUEST            = 8,
      ROUTER_ADVERTISEMENT    = 9,
      ROUTER_SOLICITATION     = 10,
      TIME_EXCEEDED           = 11,
      BAD_IP_HEADER           = 12,
      TIMESTAMP               = 13,
      TIMESTAMP_REPLY         = 14,
      INFORMATION_REQUEST     = 15,
      INFORMATION_REPLY       = 16,
      ADDRESS_MASK_REQUEST    = 17,
      ADDRESS_MASK_REPLY      = 18,
      TRACEROUTE              = 30
    };
  }
}
