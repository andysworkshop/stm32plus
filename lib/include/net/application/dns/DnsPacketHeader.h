/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Generic 12-byte DNS header. This is designed to be cast directly
     * to a memory buffer which of course means that 16 and 32 bit values are in
     * network byte order.
     */

    struct DnsPacketHeader {

      /**
       * Flags
       */

      enum Flags {
        RESPONSE              = 0x8000,
        AUTHORITATIVE_ANSWER  = 0x400,
        TRUNCATED             = 0x200,
        RECURSION_DESIRED     = 0x100,
        RECURSION_AVAILABLE   = 0x80
      };


      /**
       * Common types
       */

      enum class RecordType : uint16_t {
        A     = 1,          //!< IP address
        NS    = 2,          //!< name server
        CNAME = 5,          //!< canonical name
        PTR   = 12,         //!< pointer record
        HINFO = 13,         //!< host info
        MX    = 15          //!< mail exchange record
      };



      uint16_t dns_identification;
      uint16_t dns_flags;
      uint16_t dns_numberOfQuestions;
      uint16_t dns_numberOfAnswerRrs;
      uint16_t dns_numberOfAuthorityRrs;
      uint16_t dns_numberOfAdditionalRrs;


      /**
        * Get the total packet size, including options
        * @return
        */

      static uint16_t getPacketHeaderSize() {
        return sizeof(DnsPacketHeader);
      }
    } __attribute__((packed));
  }
}
