/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/**
 * The stm32plus namespace is the top-level namespace for all classes in the library.
 */

namespace stm32plus {

  /**
   * @brief Millisecond delay counter using the SYSTICK core peripheral
   */

  class MillisecondTimer {

    public:
      volatile static uint32_t _counter;

    public:
      static void initialise();
      static void __attribute__ ((weak)) delay(uint32_t millis_);
      static uint32_t millis();
      static void reset();
      static bool hasTimedOut(uint32_t start,uint32_t timeout);
      static uint32_t difference(uint32_t start);
  };


  /**
   * Get the current value of the systick counter.
   * @return The current value of the counter.
   */

  inline uint32_t MillisecondTimer::millis() {
    return _counter;
  }


  /**
   * Reset the counter to zero
   */

  inline void MillisecondTimer::reset() {
    _counter=0;
  }


  /**
   * Check if a timeout has been exceeded. This is designed to cope with wrap around
   * @return true if the timeout has expired
   */

  inline bool MillisecondTimer::hasTimedOut(uint32_t start,uint32_t timeout) {
    return difference(start)>timeout;
  }


  /**
   * Return the difference between a starting time and now, taking into account
   * wrap around
   * @param start when we started timing
   * @return The difference
   */

  inline uint32_t MillisecondTimer::difference(uint32_t start) {

    uint32_t now=millis();

    if(now>=start)
      return now-start;
    else
      return now+(UINT32_MAX-start+1);
  }
}

