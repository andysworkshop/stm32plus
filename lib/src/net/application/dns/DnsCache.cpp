/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net.h"


namespace stm32plus {
  namespace net {


    /**
     * Initialise the cache
     * @param cacheSize The maximum entries in the cache
     * @param rtc pointer to the RTC
     * @return true if it worked
     */

    bool DnsCache::initialise(uint32_t cacheSize,RtcBase *rtc) {

      uint32_t i;

      _maxEntries=cacheSize;
      _rtc=rtc;

      // allocate the entries

      _entries.reset(new Entry[cacheSize]);
      if(_entries.get()==nullptr)
        return false;

      // mark all as unused

      for(i=0;i<_maxEntries;i++)
        _entries[i].expiryTicks=NO_ENTRY;

      return true;
    }


    /**
     * Add a new entry to the cache. An exact match updates the expiry time. Otherwise
     * unused entries are preferred. If no unused entry is found then the entry that's
     * closest to its expiry time is evicted.
     * @param hostname The host to add
     * @param address The corresponding address
     * @param ttl Number of seconds that this address is valid for
     */

    void DnsCache::add(const char *hostname,const IpAddress& address,uint32_t ttl) {

      uint32_t i,unused,closest,closestTicks,now;
      uint16_t hostlen;
      Entry *ptr;

      unused=closest=NO_ENTRY;
      closestTicks=UINT32_MAX;
      now=_rtc->getTick();
      ptr=_entries.get();

      for(i=0;i<_maxEntries;i++,ptr++) {

        if(unused==NO_ENTRY && _entries[i].expiryTicks==NO_ENTRY)
          unused=i;
        else if(!strcasecmp(hostname,ptr->_hostname.get())) {

          // found an exact match, update host and expiry and return

          ptr->address=address;
          ptr->expiryTicks=ttl+now;

          return;
        }
        else {

          if(ptr->expiryTicks<now) {

            // this entry has expired, remove it

            ptr->expiryTicks=NO_ENTRY;
            ptr->_hostname.reset();
          }
          else if(ptr->expiryTicks-now<closestTicks) {
            closest=i;
            closestTicks=ptr->expiryTicks-now;
          }
        }
      }

      // if we have an unused entry then use it, otherwise evict.

      if(unused!=NO_ENTRY)
        i=unused;
      else
        i=closest;

      // set up the entry

      hostlen=strlen(hostname);
      _entries[i]._hostname.reset(new char[hostlen+1]);
      strcpy(_entries[i]._hostname.get(),hostname);
      _entries[i].expiryTicks=ttl+now;
      _entries[i].address=address;
    }


    /**
     * Lookup an entry in the cache
     * @param hostname
     * @param address
     * @return
     */

    bool DnsCache::lookup(const char *hostname,IpAddress& address) {

      uint32_t i;
      Entry *ptr;
      uint32_t now;

      now=_rtc->getTick();

      for(ptr=_entries.get(),i=0;i<_maxEntries;i++,ptr++) {
        if(ptr->expiryTicks!=NO_ENTRY && ptr->expiryTicks>now && !strcasecmp(hostname,ptr->_hostname.get())) {
          address=ptr->address;
          return true;
        }
      }

      return false;
    }
  }
}


#endif
