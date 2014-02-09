/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * IP subnet mask structure. Same core a.b.c.d structure as an IP address
     * but it has special interpretation as a bitmask
     */

    struct IpSubnetMask : IpAddress {

      /**
       * Default constructor
       */

      IpSubnetMask() {
      }


      /**
       * Copy constructor - ensure this gets done with a single word copy
       * @param src
       */

      IpSubnetMask(const IpSubnetMask& src)
        : IpAddress(src) {
      }


      /**
       * Construct from a dotted IP address
       * @param dottedIp The a.b.c.d address
       */

      IpSubnetMask(const char *dottedIp) {
        IpAddress::operator=(dottedIp);
      }


      /**
       * Check if this IP address is contained within the subnet managed by the
       * provided gateway. Basically they must be on the same network
       * @param ipAddress
       * @return
       */

      bool matches(const IpAddress& addr,const IpAddress& gway) const {
        return (addr.ipAddress & ipAddress)==(gway.ipAddress & ipAddress);
      }


      /**
       * Check if this is the broadcast address for this subnet. The broadcast address is found
       * by taking the network address (found by masking the incoming ip address with the subnet)
       * and or-ing it with the compliment of the subnet mask.
       * @param addr The address to be tested
       * @return true if it is
       */

      bool isBroadcastAddress(const IpAddress& addr) const {
        return ((addr.ipAddress & ipAddress) | ~ipAddress)==addr.ipAddress;
      }


      /**
       * Assignment operator
       * @param addr
       * @return self-reference
       */

      IpSubnetMask& operator=(const char *addr) {
        IpAddress::operator=(addr);
        return *this;
      }
    } __attribute__((packed));
  }
}
