/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * This class implements Peterson's algorithm for mutual exclusion. It is useful
   * where you have exactly two contenders for a resource and you know at the time
   * of calling who is who. e.g. normal code and an IRQ handler. This class has
   * the benefit of being nestable.
   */

  class CriticalSection {

    public:

      /**
       * Who wants to enter the critical section
       */

      enum class Identity : uint8_t {
        NORMAL_CODE = 0,      //!< NORMAL_CODE
        IRQ_CODE    = 1,      //!< IRQ_CODE
      };

    protected:
      volatile uint8_t _flags[2];
      volatile Identity _turn;

    public:
      CriticalSection();

      bool claim(Identity id,uint32_t millisToWait=0);
      void release(Identity id);
  };


  /**
   * Constructor
   */

  inline CriticalSection::CriticalSection() {
    _flags[0]=_flags[1]=false;
  }


  /**
   * Claim the critical section for your identity
   * @param id The identity to claim for
   * @param millisToWait The number of ms to block waiting for the critical section or zero to not block. Do not try
   *   to block if you are an IRQ source.
   * @return true if it worked, false if the critical section is busy
   */

  inline bool CriticalSection::claim(Identity id,uint32_t millisToWait) {

    Identity otherId;
    uint32_t now;

    // check that an IRQ is not trying to block

    if(id==Identity::IRQ_CODE)
      millisToWait=0;

    if(millisToWait)
      now=MillisecondTimer::millis();

    // get the other id

    otherId=id==Identity::IRQ_CODE ? Identity::NORMAL_CODE : Identity::IRQ_CODE;

    // set the entry condition

    _flags[static_cast<uint8_t>(id)]=true;
    _turn=otherId;

    // try to claim the critical section

    while(_flags[static_cast<uint8_t>(otherId)] && _turn==otherId)
      if(!millisToWait || MillisecondTimer::hasTimedOut(now,millisToWait))
        return false;

    // it's ours

    return true;
  }


  /**
   * Release the critical section
   * @param id The identity of the caller
   */

  inline void CriticalSection::release(Identity id) {
    _flags[static_cast<uint8_t>(id)]=false;
  }
}
