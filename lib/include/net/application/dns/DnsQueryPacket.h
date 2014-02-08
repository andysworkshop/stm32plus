/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * DNS query packet
     */

    struct DnsQueryPacket : DnsPacketHeader {

      /**
       * Create the query
       * @param hostname The host name
       * @param id The query identifier
       */

      uint16_t createQuery(const char *hostname,uint16_t id) {

        // we'd like the server to do the legwork

        dns_identification=NetUtil::htons(id);
        dns_flags=NetUtil::htons(RECURSION_DESIRED);
        dns_numberOfQuestions=NetUtil::htons(1);
        dns_numberOfAnswerRrs=
          dns_numberOfAuthorityRrs=
          dns_numberOfAdditionalRrs=0;

        // write out the question

        uint8_t *question=reinterpret_cast<uint8_t *>(this)+DnsPacketHeader::getPacketHeaderSize();
        uint8_t *lenptr;
        const char *srcptr;

        lenptr=question++;
        srcptr=hostname;

        while(*srcptr) {
          if(*srcptr=='.') {
            *lenptr=question-lenptr-1;
            lenptr=question++;
            srcptr++;
          }
          else
            *question++=*srcptr++;
        }

        // terminate the question label

        *lenptr=question-lenptr-1;
        *question++=0;

        // add in the type and class(1)

        *reinterpret_cast<uint16_t *>(question)=NetUtil::htons(static_cast<uint16_t>(RecordType::A));
        *reinterpret_cast<uint16_t *>(question+2)=NetUtil::htons(1);

        // return the complete query size

        return question-reinterpret_cast<uint8_t *>(this)+4;
      }

    } __attribute__((packed));
  }
}
