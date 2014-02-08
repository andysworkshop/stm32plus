/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * MAC address structure. This must be packed because it can be cast as a pointer directly
     * on to the data received in an ethernet frame
     */

    struct MacAddress {

      /**
       * the word and half-word structure enable us to do fast copy and comparisons
       */

      union {
        uint8_t macAddress[6];          //!< The MAC address bytes
        struct {
          uint32_t w;
          uint16_t hw;
        } __attribute__((packed)) macAddressWords;
      };

      /**
       * Constructor, ensure mac is zero
       */

      MacAddress() {
        memset(macAddress,0,sizeof(macAddress));
      }


      /**
       * We can beat gcc's trivial-ctor byte-by-byte copy implementation by
       * using a word and a half-word copy
       * @param src The src structure
       */

      MacAddress(const MacAddress& src) {
        macAddressWords.w=src.macAddressWords.w;
        macAddressWords.hw=src.macAddressWords.hw;
      }


      /**
       * Constructor with components
       */

      MacAddress(uint8_t f1,uint8_t f2,uint8_t f3,uint8_t f4,uint8_t f5,uint8_t f6) {
        macAddress[0]=f1;
        macAddress[1]=f2;
        macAddress[2]=f3;
        macAddress[3]=f4;
        macAddress[4]=f5;
        macAddress[5]=f6;
      }


      /**
       * Check if this is a valid (non-zero) mac address
       */

      bool isValid() const {
        return (macAddress[0] | macAddress[1] | macAddress[2] |
                macAddress[3] | macAddress[4] | macAddress[5]) !=0;
      }


      /**
       * Equality operator. Do it with two comparisons, a word and a half-word
       * @param rhs The other mac address
       * @return true if they are equal
       */

      bool operator==(const MacAddress& rhs) const {
        return macAddressWords.w==rhs.macAddressWords.w &&
               macAddressWords.hw==rhs.macAddressWords.hw;
      }
      bool operator!=(const MacAddress& rhs) const {
        return !operator==(rhs);
      }


      /**
       * Assignment.
       * @param src where to copy from
       * @return self reference
       */

      MacAddress& operator=(const MacAddress& src) {
        macAddressWords.w=src.macAddressWords.w;
        macAddressWords.hw=src.macAddressWords.hw;
        return *this;
      }


      /**
       * Create a MacAddress structure for the ethernet broadcast address
       * FF:FF:FF:FF:FF:FF
       * @return the address
       */

      static const MacAddress& createBroadcastAddress() {
        static const MacAddress mc(0xff,0xff,0xff,0xff,0xff,0xff);
        return mc;
      }


      /**
       * Create a multicast address from the IP address according to the
       * compression algorithm in RFC1112
       * @param ipGroupAddress The IP multicast group address
       */

      void createMulticastAddress(const uint8_t *ipAddressBytes) {

        // the IANA OUI plus LSB[0]=1 to indicate mcast

        macAddress[0]=0x01;
        macAddress[1]=0x00;
        macAddress[2]=0x5e;
        macAddress[3]=0x00;
        macAddress[4]=0x00;
        macAddress[5]=0x00;

        // merge in lower 23 bits of host group address into the lower 23 bits of the MAC

        macAddress[3]=ipAddressBytes[1] & 0x7F;
        macAddress[4]=ipAddressBytes[2];
        macAddress[5]=ipAddressBytes[3];
      }

    } __attribute__ ((packed));
  }
}
