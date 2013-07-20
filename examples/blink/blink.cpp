/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * This is the most basic example that uses GPIO and the
 * Systick timer to blink a LED on PF6 at 1Hz.
 *
 * If the STM32F4DISCOVERY board is your target then
 * change the GPIO declation to...
 *
 * 	GpioD<DefaultDigitalOutputFeature<13> > pd
 *
 * ... and change 2 of "pf[6]" to "pd[13]" to blink the
 * orange led on the discovery board.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 *   STM32F107VCT6
 */


class Blink {

	public:

		void run() {

			// initialise the pin for output

			GpioF<DefaultDigitalOutputFeature<6> > pf;

			// loop forever switching it on and off with a 1 second
			// delay in between each cycle

			for(;;) {

				pf[6].set();
				MillisecondTimer::delay(1000);

				pf[6].reset();
				MillisecondTimer::delay(1000);
			}
		}
};


/*
 * Main entry point
 */

int main() {

	// set up SysTick at 1ms resolution
	MillisecondTimer::initialise();

	Blink blink;
	blink.run();

	// not reached
	return 0;
}
