/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * ICMP code field
     */

    enum class IcmpCode : uint8_t {

      /// type = 0: echo reply

      ECHO_REPLY = 0,

      /// type = 3: destination unreachable

      DESTINATION_NETWORK_UNREACHABLE           = 0,
      DESTINATION_HOST_UNREACHABLE              = 1,
      DESTINATION_PROTOCOL_UNREACHABLE          = 2,
      DESTINATION_PORT_UNREACHABLE              = 3,
      FRAGMENTATION_REQUIRED                    = 4,
      SOURCE_ROUTE_FAILED                       = 5,
      DESTINATION_NETWORK_UNKNOWN               = 6,
      DESTINATION_HOST_UNKNOWN                  = 7,
      SOURCE_HOST_ISOLATED                      = 8,
      NETWORK_ADMINISTRATIVELY_PROHIBITED       = 9,
      HOST_ADMINISTRATIVELY_PROHIBITED          = 10,
      NETWORK_UNREACHABLE_FOR_TOS               = 11,
      HOST_UNREACHABLE_FOR_TOS                  = 12,
      COMMUNICATION_ADMINISTRATIVELY_PROHIBITED = 13,
      HOST_PRECEDENCE_VIOLATION                 = 14,
      PRECEDENCE_CUTOFF_IN_EFFECT               = 15,

      /// type = 4: source quench

      SOURCE_QUENCH = 0,

      /// type = 5: redirect message

      REDIRECT_DATAGRAM_FOR_THE_NETWORK         = 0,
      REDIRECT_DATAGRAM_FOR_THE_HOST            = 1,
      REDIRECT_DATAGRAM_FOR_THE_TOS_AND_NETWORK = 2,
      REDIRECT_DATAGRAM_FOR_THE_TOS_AND_HOST    = 3,

      /// type = 8: echo request

      ECHO_REQUEST = 0,

      /// type = 9: router advertisement

      ROUTER_ADVERTISEMENT = 0,

      /// type = 10: router solicitation

      ROUTER_SOLICITATION = 0,

      /// type = 11: time exceeded

      TTL_EXPIRED_IN_TRANSIT            = 0,
      FRAGMENT_REASSEMBLY_TIME_EXCEEDED = 1,

      /// type = 12: parameter problem (bad ip header)

      POINTER_INDICATES_ERROR = 0,
      MISSING_REQUIRED_OPTION = 1,
      BAD_LENGTH              = 2,

      /// type = 13: timestamp

      TIMESTAMP = 0,

      /// type = 14: timestamp reply

      TIMESTAMP_REPLY = 0,

      /// type = 15: information request

      INFORMATION_REQUEST = 0,

      /// type = 16: information reply

      INFORMATION_REPLY = 0,

      /// type = 17: address mask request

      ADDRESS_MASK_REQUEST = 0,

      /// type = 18: address mask reply

      ADDRESS_MASK_REPLY = 0,

      /// type = 30: traceroute

      TRACEROUTE_INFORMATION_REQUEST = 0
    };
  }
}
