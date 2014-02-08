/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Packet definition for ICMP v4. This structure can be cast directly on to the
     * payload of an incoming IpPacket.
     */

    struct IcmpPacket {
      IcmpType icmp_type;             // uint8_t
      IcmpCode icmp_code;             // uint8_t
      uint16_t icmp_checksum;

    } __attribute__((packed));


    /**
     * Echo reply structure (0/0)
     */

    struct IcmpEchoReply : IcmpPacket {

      uint16_t icmp_identifier;
      uint16_t icmp_sequenceNumber;
      uint8_t icmp_data[1];

      void initialise() {
        icmp_type=IcmpType::ECHO_REPLY;
        icmp_code=IcmpCode::ECHO_REPLY;
        icmp_checksum=0;
      }

      static uint32_t getHeaderSize() {
        return sizeof(IcmpEchoReply)-1;
      }
    } __attribute__((packed));


    /**
     * Echo request packet definition (8/0)
     */

    struct IcmpEchoRequest : IcmpPacket {

      uint16_t icmp_identifier;
      uint16_t icmp_sequenceNumber;
      uint8_t icmp_data[1];

      void initialise() {
        icmp_type=IcmpType::ECHO_REQUEST;
        icmp_code=IcmpCode::ECHO_REQUEST;
        icmp_checksum=0;
      }

      static uint32_t getHeaderSize() {
        return sizeof(IcmpEchoRequest)-1;
      }

      static bool isPacket(IcmpPacket& icmpPacket) {
        return icmpPacket.icmp_type==IcmpType::ECHO_REQUEST && icmpPacket.icmp_code==IcmpCode::ECHO_REQUEST;
      }
    } __attribute__((packed));


    /**
     * Destination unreachable (3/X)
     */

    struct IcmpDestinationUnreachable : IcmpPacket {

      uint16_t icmp_unused;
      uint16_t icmp_nextHopMtu;

      void initialise(IcmpCode code) {
        icmp_type=IcmpType::DESTINATION_UNREACHABLE;
        icmp_code=code;
        icmp_checksum=0;
        icmp_unused=icmp_nextHopMtu=0;
      }

      static uint32_t getHeaderSize() {
        return sizeof(IcmpDestinationUnreachable);
      }

      static bool isPacket(IcmpPacket& icmpPacket) {
        return icmpPacket.icmp_type==IcmpType::DESTINATION_UNREACHABLE;
      }
    } __attribute__((packed));
  }
}
