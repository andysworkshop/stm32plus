/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * IP address structure
     */

    struct IpAddress {
      union {
        uint32_t ipAddress;               // big-endian. 192.0.2.235 = 0xC00002EB
        uint8_t ipAddressBytes[4];        // e.g. 0xC0 0x00 0x02 0xEB
      };


      /**
       * Constructor, ensure address is zero when we start up
       */

      IpAddress() : ipAddress(0) {
      }


      /**
       * Copy constructor - ensure this gets done with a single word copy
       * @param src
       */

      IpAddress(const IpAddress& src) {
        ipAddress=src.ipAddress;
      }


      /**
       * Construct from a dotted IP address
       * @param dottedIp The a.b.c.d address
       */

      IpAddress(const char *dottedIp) {
        operator=(dottedIp);
      }


      /**
       * Check if this is a valid (non-zero) address
       */

      bool isValid() const {
        return ipAddress!=0;
      }


      /**
       * Check if this is the broadcast address 255.255.255.255
       * @return true if it is the broadcast address
       */

      bool isBroadcast() const {
        return ipAddress==0xFFFFFFFF;
      }


      /**
       * Check if this is the all-hosts multicast group (224.0.0.1)
       * @return true if it matches
       */

      bool isAllHostsMulticastGroup() const {
        return ipAddress==0x010000E0;
      }


      /**
       * Check if this is an address in any of the host groups
       */

      bool isMulticastGroup() const {
        return NetUtil::ntohl(ipAddress)>=0xE0000001 && NetUtil::ntohl(ipAddress)<=0xEFFFFFFF;
      }


      /**
       * Invalidate this address
       */

      void invalidate() {
        ipAddress=0;
      }


      /**
       * Equality operator
       */

      bool operator==(const IpAddress& rhs) const {
        return ipAddress==rhs.ipAddress;
      }
      bool operator!=(const IpAddress& rhs) const {
        return !operator==(rhs);
      }


      /**
       * Set up from a dotted quad "a.b.c.d". The format is not error checked.
       * @param addr The dotted quad address
       * @return a self reference
       */

      IpAddress& operator=(const char *addr) {

        uint8_t *ptr,index;

        index=0;
        ptr=ipAddressBytes;
        *ptr=0;

        while(index<4 && *addr) {

          if(isdigit(*addr))
            *ptr=(*ptr*10)+(*addr)-'0';
          else if(*addr=='.') {
            ptr++;
            *ptr=0;
          }

          addr++;
        }

        return *this;
      }


      /**
       * Assignment
       * @param rhs Copy source
       * @return self reference
       */

      IpAddress& operator=(const IpAddress& rhs) {
        ipAddress=rhs.ipAddress;
        return *this;
      }

      /**
       * Returning a reference causes gcc to emit a bogus warning
       * @param rhs
       */

      void operator=(const volatile IpAddress& rhs) volatile {
        ipAddress=rhs.ipAddress;
      }


      /**
       * Assign 127.0.0.1 to this address
       */

      void setLocalhost() {
        ipAddress=0x7f000001;
      }


      /**
       * Check if this IP address is localhost
       * @return true if it is localhost
       */

      bool isLocalhost() const {
        return ipAddress==0x7f000001;
      }


      /**
       * Check if this IP address is on the local class A network (127.X.X.X)
       * @return true if it's on the local net
       */

      bool isLocalNetwork() const {
        return ipAddressBytes[0]==127;
      }


      /**
       * Write to a user-supplied buffer as A.B.C.D
       * @param buffer Where to write
       */

      void toString(char *buffer) const {

        uint8_t i;

        for(i=0;i<4;i++) {

          StringUtil::modp_uitoa10(ipAddressBytes[i],buffer);
          buffer+=strlen(buffer);

          if(i!=3)
            *buffer++='.';
        }
      }

    } __attribute__((packed));
  }
}
