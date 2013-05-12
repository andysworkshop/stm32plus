/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Demo of the DMA peripheral used to fill a block of
 * memory with a single byte. We will repeatedly fill a
 * buffer with 0xAA and check that it's done OK. If so
 * then a LED on PF6 will blink once per second.
 *
 * DMA1, channel 4 is used for this demo for F1
 * devices and DMA2, channel3, stream 4 is used for
 * F4 devices.
 *
 * If this example is to be run on the STM32F4DISCOVERY
 * board then change the LED configuration from PF6 to
 * PD13 and invert the set() / reset() logic because
 * that LED is active HIGH.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class DmaFillTest {

	protected:

		/**
		 * The LED is on PF6
		 */

		enum { LED_PIN = 6 };

	public:

		void run() {

			uint8_t buffer[256],byteToFill;
			int i;

			// initialise the LED pin

			GpioF<DefaultDigitalOutputFeature<LED_PIN> > pf;

			// lights off (this LED is active low, i.e. PF6 is a sink)

			pf[LED_PIN].set();

			// declare a DMA channel with the memory fill feature
			// F4 users note that only DMA2 can do memory-to-memory transfers.

#if defined(STM32PLUS_F1)

			Dma1Channel4<
				DmaMemoryFillFeature<> 				// memory fill with default transfer size (bytes)
			> dma;

#elif defined(STM32PLUS_F4)

			Dma2Channel3Stream4<
				DmaMemoryFillFeature<> 				// memory fill with default transfer size (bytes)
			> dma;

#endif

			byteToFill=0xaa;

			for(;;) {

				// clear the target buffer

				memset(buffer,'\0',sizeof(buffer));

				// start the transfer of 256 bytes from buffer1 to buffer2. this executes asynchronously.

				dma.beginCopyMemory(buffer,&byteToFill,sizeof(buffer),DMA_Priority_Medium);

				// wait for transfer complete

				dma.waitUntilComplete();

				// verify the result

				for(i=0;i<256;i++)
					if(buffer[i]!=0xaa)
						for(;;);					// lock up on error

				// flash the LED for a second

				pf[LED_PIN].reset();
				MillisecondTimer::delay(1000);
				pf[LED_PIN].set();
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

	DmaFillTest test;
	test.run();

	// not reached
	return 0;
}
