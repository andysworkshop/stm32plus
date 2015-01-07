/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "config/debug.h"

//Comment if not on debug, if it is used while not debugging it will raise a HardFault
#define DEBUG_ON

using namespace stm32plus;


/**
 * This is a basic example of using the SemiHosting
 * capabilities.
 * It is based on the blink example, if you wish to 
 * see the led blink change the Gpio as normal
 *
 * Quoted from the blink.cpp:
 * 
 * If the STM32F4DISCOVERY board is your target then
 * change the GPIO declation to...
 *
 *  GpioD<DefaultDigitalOutputFeature<13> > pd
 *
 * ... and change 2 of "pc[8]" to "pd[13]" to blink the
 * orange led on the discovery board.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F030R8T6
 */

class Blink {

  public:

    void run() {

      // initialise the pin for output

      GpioC<DefaultDigitalOutputFeature<8> > pc;

      // loop forever switching it on and off with a 1 second
      // delay in between each cycle
	  int i=0; 
      for(;;) {
		#if defined(DEBUG_ON)
			SemiHosting::out << "On the infinite for. Run #"<<i<<"\n"; //print a variable.
		#endif
        pc[8].set();
        MillisecondTimer::delay(1000);

        pc[8].reset();
        MillisecondTimer::delay(1000);
      }
    }
};


/*
 * Main entry point
 */

int main() {

#if defined(DEBUG_ON)
  SemiHosting::out << "Start!\n";
  SemiHosting::puts("This way also works!\n");
#endif

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  Blink blink;
  blink.run();

  // not reached
  return 0;
}
