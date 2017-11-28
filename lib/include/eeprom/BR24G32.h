/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * The ROHM BR24G32 is functionally the same as the ATMEL AT24C32
   */

  template<class TI2C> using BR24G32=AT24Cxx<TI2C,4096>;
}
