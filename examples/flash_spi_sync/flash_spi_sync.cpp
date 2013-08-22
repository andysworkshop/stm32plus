/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/flash/spi.h"


using namespace stm32plus;


/**
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class FlashSpiSync {

	public:

		void run() {

			// we'll use SPI1

			typedef Spi1<> MySpi;
			MySpi::Parameters spiParams;

			// we're the master

			spiParams.spi_mode=SPI_Mode_Master;

			// declare and initialise the peripheral

			MySpi spi(spiParams);

			// declare the flash device to use the SPI1 peripheral

			spiflash::W25Q16DW<MySpi> flash(spi);
		}
};


/*
 * Main entry point
 */

int main() {

	MillisecondTimer::initialise();

	FlashSpiSync test;
	test.run();

	// not reached
	return 0;
}
