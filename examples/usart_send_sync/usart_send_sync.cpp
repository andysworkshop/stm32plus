/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: synchronous sending data
 *
 * This example will send the string "Hello World" using
 * USART1. Connect an RS232 cable from USART1 to your PC
 * and run a terminal program (there are many free
 * terminal programs) to see the data. The default
 * (unremapped) USART1 pins are used.
 *
 * Note that if you are using the STM32F4DISCOVERY board
 * then you cannot use Usart1 since the pins clash with
 * onboard peripherals. I have tested this code on that
 * board using Usart2.
 *
 * The protocol is 57600/8/N/1
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class UsartSendSyncTest {

	public:

		void run() {

			const char *dataToSend="Hello World";

			/*
			 * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
			 * if your application demands that you use the alternate pins for USART1
			 */

			Usart1<> usart(57600);

			/*
			 * For kicks we'll use an output stream for sending to the port instead of using the
			 * send(uint8_t) method on the usart object
			 */

			UsartPollingOutputStream outputStream(usart);

			/*
			 * Send the data
			 */

			if(!outputStream.write(dataToSend,strlen(dataToSend))) {
				// error handling would go here
			}

			// finished

			for(;;);
		}
};


/*
 * Main entry point
 */

int main() {

	UsartSendSyncTest test;
	test.run();

	// not reached
	return 0;
}
