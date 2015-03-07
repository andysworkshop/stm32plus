/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"


#if defined(STM32PLUS_F4)


using namespace stm32plus;


/**
 * Delay for the given number of milliseconds
 */

extern "C" {
  void HAL_Delay(uint32_t ms) {
    MillisecondTimer::delay(ms);
  }
}

#endif
