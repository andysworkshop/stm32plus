/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/flash/spi.h"
#include "config/display/tft.h"


using namespace stm32plus;
using namespace stm32plus::display;


/**
 * This example is closely related to the 'flash_spi_program' example. That example programmed
 * the flash IC with 10 sample graphics files. This example will read back those graphics
 * files and display them on a TFT LCD.
 *
 * In this example the LCD is an LG KF700 cellphone LCD with an HX8352A controller. You can change
 * the graphics driver to a different one if you need to.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class FlashSpiReader {

	// definitions for the LCD panel

	typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
	typedef ILI9325_Landscape_262K<LcdAccessMode> LcdPanel;

	LcdAccessMode *_accessMode;
	LcdPanel *_gl;

	// these are the peripherals we will use for the flash access

	typedef Spi1<> MySpi;
	typedef spiflash::StandardSpiFlashDevice<MySpi> MyFlash;

	// declare the peripheral pointers

	MySpi *_spi;
	MyFlash *_flash;

	public:

		void run() {

			// Initialise the SPI peripheral in master mode. The SPI speed is the highest available.
			// Make sure that this is not too fast for your flash device.

			MySpi::Parameters spiParams;
			spiParams.spi_mode=SPI_Mode_Master;
			spiParams.spi_baudRatePrescaler=SPI_BaudRatePrescaler_2;

			_spi=new MySpi(spiParams);

			// initialise the flash device

			_flash=new MyFlash(*_spi);

			// LCD reset is on PE1 and LCD RS (D/CX) is on PD11

			GpioE<DefaultDigitalOutputFeature<1> > pe;
			GpioD<DefaultFsmcAlternateFunctionFeature<11>> pd;

			// set up the FSMC timing. these numbers (particularly the data setup time) are dependent on
			// both the FSMC bus speed and the panel timing parameters.

			Fsmc8080LcdTiming fsmcTiming(0,2);

			// set up the FSMC with RS=A16 (PD11)

			_accessMode=new LcdAccessMode(fsmcTiming,16,pe[1]);
			_gl=new LcdPanel(*_accessMode);

			// apply gamma settings

			ILI9325Gamma gamma(0x0006,0x0101,0x0003,0x0106,0x0b02,0x0302,0x0707,0x0007,0x0600,0x020b);
			_gl->applyGamma(gamma);

			// clear to black while the lights are out

			_gl->setBackground(0);
			_gl->clearScreen();

			// create the backlight on timer4, channel2 (PD13)

			DefaultBacklight backlight;

			// fade up to 100% in 4ms steps

			backlight.fadeTo(100,4);

			// the 'flash_spi_program' example wrote a sequence of 10 JPEG files to the SPI
			// flash device. For convenience this demo hard-codes the size and location.

			for(;;) {

				copyJpegToLcd(0,999,Rectangle(0,0,320,240));
				///etc...
			}
		}


		/*
		 * Copy a JPEG from flash to the LCD
		 */

		void copyJpegToLcd(uint32_t offset,uint32_t size,const Rectangle& rc) {


		}


		/*
		 * Verify the file just written to the device
		 */

		void verifyFile(const FlashEntry& fe) {

			uint8_t filePage[MyFlash::PAGE_SIZE],flashPage[MyFlash::PAGE_SIZE];
			scoped_ptr<File> file;
			uint32_t remaining,actuallyRead,address;

			*_usartStream << "Verifying " << fe.filename << "\r\n";

			if(!_fs->openFile(fe.filename,file.address()))
				error("Failed to open file");

			address=fe.offset;

			for(remaining=fe.length;remaining;remaining-=actuallyRead) {

				// read a page from the file

				if(!file->read(filePage,sizeof(filePage),actuallyRead))
					error("Failed to read from file");

				// cannot hit EOF here

				if(!actuallyRead)
					error("Unexpected end of file");

				// read the page from the flash device

				if(!_flash->read(address,flashPage,actuallyRead))
					error("Failed to read from the flash device");

				// compare it

				if(memcmp(filePage,flashPage,actuallyRead)!=0)
					error("Verify error: programming failed");

				// update for next

				address+=actuallyRead;
			}
		}


		void error(const char *text) {
			for(;;);
		}
};


/*
 * Main entry point
 */

int main() {

	MillisecondTimer::initialise();

	FlashSpiReader test;
	test.run();

	// not reached
	return 0;
}
