/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "concurrent/Mutex.h"
#include "timing/MillisecondTimer.h"


#if !defined(STM32PLUS_F0)

namespace stm32plus {


  /**
   * Constructor, set flag to unclaimed
   */

  Mutex::Mutex() {
    _flag=0;
  }


  /**
   * Try to claim the mutex, give up after the specified number of millis
   * @param millisToWait max time to wait, set to zero to never block
   * @return true if was claimed
   */

  bool Mutex::claim(uint32_t millisToWait) {

    uint8_t oldval,newval;
    uint32_t now;

    if(millisToWait)
      now=MillisecondTimer::millis();

    do {

      // get the current mutex value

      if((oldval=__LDREXB(&_flag))==0) {

        // try to claim it and return if OK

        if((newval=__STREXB(1,&_flag))==0) {
          __DMB();
          return true;
        }
      }

      // abort if not allowed to wait

      if(!millisToWait)
        return false;

    } while(!MillisecondTimer::hasTimedOut(now,millisToWait));

    return false;
  }


  /**
   * Release a locked mutex
   */

  void Mutex::release() {
    _flag=0;
  }
}

#endif
