/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Header of an IPv4 packet. Not documented here, documentation abounds on the internet.
     */

    struct IpPacketHeader {

      uint8_t ip_hdr_version;
      uint8_t ip_hdr_typeOfService;
      uint16_t ip_hdr_length;
      uint16_t ip_hdr_identification;
      uint16_t ip_hdr_flagsAndOffset;
      uint8_t ip_hdr_ttl;
      IpProtocol ip_hdr_protocol;
      uint16_t ip_hdr_checksum;

      IpAddress ip_sourceAddress;
      IpAddress ip_destinationAddress;

      /**
       * Get the size of an IP header with no options
       */

      constexpr static uint16_t getNoOptionsHeaderSize() {
        return 20;
      }
    } __attribute__((packed));      // 20 bytes.
  }
}
