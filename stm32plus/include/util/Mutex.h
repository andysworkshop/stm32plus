/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Mutex class using the ARM primitive instructions
   */

  class Mutex {

    protected:
      volatile uint8_t _flag;

    public:
      Mutex();

      bool claim(uint32_t millisToWait);
      void release();
  };
}
