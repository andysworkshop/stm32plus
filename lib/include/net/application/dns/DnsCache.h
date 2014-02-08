/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Simple DNS cache of hostname to IP address with TTL. A full pass through the cache is
     * required for add and lookup so this is not intended for high volumes, not that you'd
     * have the SRAM for that anyway. A pessimistic average domain name length of 25 characters
     * with 20 entries would use around (26*20)+(12*20) = 760 bytes of SRAM in the cache.
     * Whatever, parsing 20 entries per lookup is massively cheaper than a network DNS lookup.
     */

    class DnsCache {

      protected:

        /**
         * Constant to indicate the cache entry is free
         */

        enum {
          NO_ENTRY = 0xffffffff       //!< The cache entry is free
        };

        struct Entry {
          scoped_array<char> _hostname;
          uint32_t expiryTicks;
          IpAddress address;
        };

        scoped_array<Entry> _entries;
        uint32_t _maxEntries;
        RtcBase *_rtc;

      public:
        bool initialise(uint32_t cacheSize,RtcBase *rtc);

        void add(const char *hostname,const IpAddress& address,uint32_t expiry);
        bool lookup(const char *hostname,IpAddress& address);
    };
  }
}

