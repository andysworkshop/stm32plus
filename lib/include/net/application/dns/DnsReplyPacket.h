/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * DNS reply packet
     */

    struct DnsReplyPacket : DnsPacketHeader {

      /**
       * Find the start of the answers in this packet. Assumes a well-formed response.
       * @return the answers pointer
       */

      uint8_t *findAnswers() {

        uint16_t i;
        uint8_t *ptr;

        // get a pointer to the questions

        ptr=reinterpret_cast<uint8_t *>(this)+DnsPacketHeader::getPacketHeaderSize();

        // step over each question

        for(i=NetUtil::ntohs(dns_numberOfQuestions);i!=0;i--)
          ptr=stepOverName(ptr)+4;          // step over the name + (type, class)

        return ptr;
      }


      /**
       * Step over the labels that make up the name pointed at by ptr. The labels that make up a
       * name may optionally end in a pointer, or just be a pointer. In any case the occurence of
       * a pointer is the end of the name.
       * @param ptr The start of a name
       * @return The start of the next name
       */

      uint8_t *stepOverName(uint8_t *ptr) {

        while(*ptr) {

          // check for a pointer

          if((*ptr & 0xc0)==0xc0)
            return ptr+2;           // don't care what the pointer is, just that this is the end of the name

          ptr+=(*ptr)+1;            // normal label with length prefix
        }

        // and over the \0 terminator

        return ptr+1;
      }


      /**
       * Find the first occurrence of a record with the given type
       * @param answers The start of the answers records
       * @param requestedType The type to search for
       * @return A pointer to the record, or null
       */

      uint8_t *findRecord(uint8_t *answers,RecordType requestedType) {

        uint8_t *dataptr,*ptr;
        RecordType thisType;
        uint16_t i;

        ptr=answers;

        for(i=NetUtil::ntohs(dns_numberOfAnswerRrs);i!=0;i--) {

          // get past the encoded name

          dataptr=stepOverName(ptr);

          // is it a record of the correct type?

          thisType=static_cast<RecordType>(NetUtil::ntohs(*reinterpret_cast<uint16_t *>(dataptr)));
          if(thisType==requestedType)
            return ptr;

          // next one

          ptr=dataptr+8;                                                // to the resource data length
          ptr+=2+NetUtil::ntohs(*reinterpret_cast<uint16_t *>(ptr));    // over the length and data
        }

        // not found

        return nullptr;
      }

    } __attribute__((packed));
  }
}

