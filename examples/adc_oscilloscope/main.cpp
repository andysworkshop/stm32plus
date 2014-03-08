/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "stdafx.h"


using namespace stm32plus;


int main() {

  // we're using interrupts, initialise NVIC and initialise
  // the SysTick millisecond timer used internally.

  Nvic::initialise();
  MillisecondTimer::initialise();

  // declare and run the application

  Oscilloscope scope;
  scope.run();

  // not reached
  return 0;
}
