/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Mutex class using the ARM primitive LRDEX/STREX instructions.
   * Do not nest Mutex's. i.e. Do not call claim() on Mutex #1 and then call claim() somewhere
   * else on Mutex #2 before Mutex #1 has been released. The ARM synchronisation primitives
   * will automatically release #1 if you do that.
   */

  class Mutex {

    protected:
      volatile uint8_t _flag;

    public:
      Mutex();

      bool claim(uint32_t millisToWait=0);
      void release();
  };
}
