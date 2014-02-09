/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Simple LRU cache for an ARP mapping of IP address to MAC address. Implemented as
     * a compact doubly linked list with a fixed maximum number of entries. The list
     * entry overhead is just 2 bytes and no dynamic memory allocation is required after
     * initialisation. Searching is a linear operation from the MRU to the LRU so the
     * number of entries should be kept low. The maximum number of entries is 255.
     * Index #255 is reserved.
     */

    class ArpCache {

      protected:

        /**
         * Constant for no entry at this position
         */

        enum {
          NO_ENTRY = 0xff                 ///< used as a next/prev marker to say 'none'
        };

        struct CacheEntry {
          uint8_t next;                   ///< next index in the list (or NO_ENTRY)
          uint8_t previous;               ///< previous index in the list (or NO_ENTRY)
          MacAddress macAddress;          ///< mac address held here
          IpAddress ipAddress;            ///< ip address held here
          uint32_t expiryTime;            ///< RTC time after which this entry is invalid
        } __attribute__((packed));

        uint8_t _first;                   ///< first index in the list (or NO_ENTRY if empty)
        uint8_t _last;                    ///< last index in the list (or NO_ENTRY if empty)
        uint8_t _insertionPoint;          ///< next free array slot for new entries (or _maxEntries-1 if full)
        uint8_t _maxEntries;              ///< total entries allowed
        uint32_t _expirySeconds;          ///< seconds to keep in a cache
        RtcBase *_rtc;                    ///< Pointer to the RTC

        scoped_array<CacheEntry> _array;  ///< the array of CacheEntry structures

        volatile bool _watchFlag;         ///< set to true when _watchIp is inserted
        IpAddress _watchIp;             ///< an IP to watch for
        MacAddress *_foundMac;            ///< MAC address found for the watcher

      protected:
        void internalInsert(const MacAddress& mac,const IpAddress& ip);
        void unlink(uint8_t index);
        void insertFront(uint8_t index);
        bool hasExpired(uint8_t index) const;

      public:
        bool initialise(uint8_t numEntries,uint32_t expirySeconds,RtcBase *rtc);

        void insert(const MacAddress& mac,const IpAddress& ip);
        bool findMacAddress(const IpAddress& ip,MacAddress& found);

        void setWatchIp(const IpAddress& ip,MacAddress *foundMac);
        bool waitForWatch(uint32_t timeout);
    };


    /**
     * Initialise the cache. Memory is created for the array of entries
     * up front to avoid churn of the heap that may occur during activity
     * if entries were dynamically allocated.
     * @param numEntries The maximum number of entries
     * @param expirySeconds Max time to keep an entry in the cache. An entry will still be
     *   evicted if it's the LRU and a new entry comes in to a full cache.
     * @param rtc A pointer to the RTC used to get the current time
     * @return true if it works
     */

    inline bool ArpCache::initialise(uint8_t numEntries,uint32_t expirySeconds,RtcBase *rtc) {

      _array.reset(new CacheEntry[numEntries]);

      // initialise internal variables

      _first=_last=NO_ENTRY;
      _insertionPoint=0;
      _maxEntries=numEntries;
      _expirySeconds=expirySeconds;
      _rtc=rtc;
      _watchFlag=false;

      return _array.get()!=nullptr;
    }


    /**
     * Insert a new entry or update an old one. This is IRQ-safe
     * @param mac The MAC address to insert
     * @param ip The associated IP address
     */

    inline void ArpCache::insert(const MacAddress& mac,const IpAddress& ip) {

      // protect ourselves from re-entrancy while we do the insert

      IrqSuspend suspender;
      internalInsert(mac,ip);
    }

    inline void ArpCache::internalInsert(const MacAddress& mac,const IpAddress& ip) {

      uint8_t i,found,unused;
      uint32_t timeNow;
      CacheEntry *ptr;

      // can we release a watcher when this IRQ finishes?

      if(_watchFlag && _watchIp==ip) {
        *_foundMac=mac;
        _watchFlag=false;
      }

      // don't care for the link order here, just walk the array. If a matching
      // cache entry is found then get out right away. We also need to take care
      // of a change of association so we also try to find matches by IP and MAC
      // but not both

      ptr=_array.get();
      found=unused=NO_ENTRY;
      timeNow=_rtc->getTick();

      for(i=0;i<_insertionPoint;i++) {

        if(ptr->next==NO_ENTRY && ptr->previous==NO_ENTRY && i!=_first)
          unused=i;
        else {

          // if expired then remove it

          if(timeNow>ptr->expiryTime) {
            unlink(i);
            unused=i;
          }
          else if(ptr->ipAddress==ip) {

            if(ptr->macAddress==mac)
              found=i;        // found an exact match that hasn't expired
            else {

              // IP address is cached but has been assigned to another MAC
              // this cache entry must be expunged

              unlink(i);
              unused=i;
            }
          }
          else if(ptr->macAddress==mac) {

            // MAC address is in the cache but has a different IP address
            // the cache entry is not valid any more

            unlink(i);
            unused=i;
          }
        }
        ptr++;
      }

      // so what happened? if an unexpired match was found then it needs to come to the front

      if(found!=NO_ENTRY) {

        // take the entry out of the list and put it back at the front

        unlink(found);
        insertFront(found);

        return;
      }

      // a new entry needs to be created. can we place it in a previously
      // used position?

      if(unused!=NO_ENTRY)
        i=unused;
      else if(_insertionPoint!=_maxEntries) {

        // there is space to put it on the end

        i=_insertionPoint;
        _insertionPoint++;
      }
      else {

        // there is no space, evict the LRU entry at the end of the cache

        i=_last;
        unlink(_last);
      }

      // enter our details

      ptr=&_array[i];
      ptr->macAddress=mac;
      ptr->ipAddress=ip;
      ptr->expiryTime=_rtc->getTick()+_expirySeconds;

      // we are the most recent, insert at front

      insertFront(i);
    }


    /**
     * Find the MAC address given an IP address. The cached address must not have
     * expired. Expired entries are intentionally not cleaned here to keep the
     * performance to a maximum. Expired entries are detected and recycled in the
     * insert() method.
     * @param ip The IP address to find
     * @param found The found mac address
     * @return true if found
     */

    inline bool ArpCache::findMacAddress(const IpAddress& ip,MacAddress& found) {

      uint8_t i;
      bool ret;

      ret=false;

      // protect ourselves from re-entrancy

      IrqSuspend suspender;

      for(i=_first;i!=NO_ENTRY;i=_array[i].next) {
        if(_array[i].ipAddress==ip && !hasExpired(i)) {
          found=_array[i].macAddress;
          ret=true;
          break;
        }
      }

      return ret;
    }


    /**
     * Check if this ARP cache entry has expired
     * @param index The index to check
     * @return true if it's expired
     */

    inline bool ArpCache::hasExpired(uint8_t index) const {
      return _rtc->getTick()>_array[index].expiryTime;
    }


    /**
     * Set a watch IP address (will be triggered when inserted)
     * @param ip The IP address to watch for
     * @param foundMac Where to store the found MAC result
     */

    inline void ArpCache::setWatchIp(const IpAddress& ip,MacAddress *foundMac) {
      _foundMac=foundMac;
      _watchIp=ip;
      _watchFlag=true;
    }


    /**
     * Wait for a configurable time for a watch ip to be triggered. The idea is that an ARP
     * frame is received over an IRQ and added to the cache, releasing the main CPU that is
     * waiting on the watch flag.
     * @param timeout The timeout value in ms
     * @return true if found in time
     */

    inline bool ArpCache::waitForWatch(uint32_t timeout) {

      uint32_t now;

      now=MillisecondTimer::millis();

      while(_watchFlag)
        if(MillisecondTimer::hasTimedOut(now,timeout))
          return false;

      return true;
    }


    /**
     * Unlink an entry from the list
     * @param index The entry to unlink
     */

    inline void ArpCache::unlink(uint8_t index) {

      CacheEntry *ptr;

      ptr=&_array[index];

      if(_first==index)
        _first=ptr->next;

      if(_last==index)
        _last=ptr->previous;

      if(ptr->previous!=NO_ENTRY)
        _array[ptr->previous].next=ptr->next;

      if(ptr->next!=NO_ENTRY)
        _array[ptr->next].previous=ptr->previous;

      ptr->next=NO_ENTRY;
      ptr->previous=NO_ENTRY;
    }


    /**
     * Link an entry into the front of the list
     * @param index The index to link in
     */

    inline void ArpCache::insertFront(uint8_t index) {

      // if there is already a first then it's now the second

      if(_first!=NO_ENTRY)
        _array[_first].previous=index;

      // set up our links

      _array[index].previous=NO_ENTRY;
      _array[index].next=_first;

      // if there was no last then there is now

      if(_last==NO_ENTRY)
        _last=index;

      // we are the new first

      _first=index;
    }
  }
}
