/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/flash/spi.h"
#include "config/sdcard.h"
#include "config/usart.h"
#include "config/filesystem.h"
#include "memory/scoped_ptr.h"
#include <vector>
#include <string>


using namespace stm32plus;


/**
 * This example programs a SPI flash device from files stored on an SD card and outputs
 * progress messages to a USART terminal. This programmer only requires the standard SPI
 * commands so it uses the 'StandardSpiFlashDevice' template. Other templates are available
 * that mixin commands specific to those devices.
 *
 * The SD card must contain an "index.txt" file in the root folder. "index.txt" contains
 * one line per file to flash The line is of the form:
 *
 *   <filename>=<start-address-in-flash-in-decimal>
 *
 * For example:
 *
 *   /graphic1.bin=0
 *   /graphic2.bin=16384
 *   /assets/line.bin=24576
 *
 * Whitespace is not permitted anywhere on the text lines. It is important that each address
 * is a multiple of the device page size (usually 256 bytes). If it's not then you will get
 * data corruption. A chip-erase command is used to wipe the device before
 * programming.
 *
 * An example "spiflash" directory is included with this example that can be copied to
 * the root of your SD card. See the related 'flash_spi_reader' example for a demo that
 * reads back the example graphic files and displays them on an LCD.
 *
 * The default peripherals for this demo are SPI1, USART1, Winbond W25Q16DW 16Mbit flash. All
 * of these are customisable by you.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class FlashSpiProgram {

	// these are the peripherals we will use

	typedef Spi1<> MySpi;
	typedef Usart1<> MyUsart;
	typedef spiflash::StandardSpiFlashDevice<MySpi> MyFlash;

	// declare the peripheral pointers

	MyUsart *_usart;
	MySpi *_spi;
	MyFlash *_flash;
	SdioDmaSdCard *_sdcard;
	FileSystem *_fs;
	UsartPollingOutputStream *_usartStream;

	// declare the program variables

	struct FlashEntry {
		char *filename;
		uint32_t length;
		uint32_t offset;
	};

	std::vector<FlashEntry> _flashEntries;

	public:

		void run() {

			// initialise the USART

			_usart=new MyUsart(57600);
			_usartStream=new UsartPollingOutputStream(*_usart);

			// initialise the SD card

			_sdcard=new SdioDmaSdCard;

			if(errorProvider.hasError())
				error("SD card could not be initialised");

			// initialise the filesystem on the card

			NullTimeProvider timeProvider;

			if(!FileSystem::getInstance(*_sdcard,timeProvider,_fs))
				error("The file system on the SD card could not be initialised");

			// Initialise the SPI peripheral in master mode. The SPI speed is the highest available.
			// Make sure that this is not too fast for your device.

			MySpi::Parameters spiParams;
			spiParams.spi_mode=SPI_Mode_Master;
			spiParams.spi_baudRatePrescaler=SPI_BaudRatePrescaler_2;

			_spi=new MySpi(spiParams);

			// initialise the flash device

			_flash=new MyFlash(*_spi);

			// read the index file

			readIndexFile();

			// erase the flash device

			eraseFlash();

			// write each file

			for(auto it=_flashEntries.begin();it!=_flashEntries.end();it++) {
				writeFile(*it);
				verifyFile(*it);
			}

			// done

			status("Success");
			for(;;);
		}


		/*
		 * Erase the entire device
		 */

		void eraseFlash() {

			status("Erasing the entire flash device");

			if(!_flash->writeEnable())
				error("Unable to enable write access");

			if(!_flash->chipErase())
				error("Unable to execute the erase command");

			if(!_flash->waitForIdle())
				error("Failed to wait for the flash device to be idle");

			status("Erase completed");
		}


		/*
		 * Write the file to the flash device
		 */

		void writeFile(const FlashEntry& fe) {

			uint8_t page[MyFlash::PAGE_SIZE];
			scoped_ptr<File> file;
			uint32_t remaining,actuallyRead,address;

			*_usartStream << "Programming " << fe.filename << "\r\n";

			if(!_fs->openFile(fe.filename,file.address()))
				error("Failed to open file");

			address=fe.offset;

			for(remaining=fe.length;remaining;remaining-=actuallyRead) {

				// read a page from the file

				if(!file->read(page,sizeof(page),actuallyRead))
					error("Failed to read from file");

				// cannot hit EOF here

				if(!actuallyRead)
					error("Unexpected end of file");

				// wait for the device to go idle

				if(!_flash->waitForIdle())
					error("Failed to wait for the device to become idle");

				// enable writing

				if(!_flash->writeEnable())
					error("Unable to enable write access");

				// program the page

				if(!_flash->pageProgram(address,page,actuallyRead))
					error("Failed to program the page");

				// update for next

				address+=actuallyRead;
			}
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


		/*
		 * Read index.txt
		 */

		void readIndexFile() {

			scoped_ptr<File> file;
			char line[200],*ptr;

			// open the file

			if(!_fs->openFile("/index.txt",file.address()))
				error("Cannot open /index.txt");

			// attach a reader and read each line

			FileReader reader(*file);

			while(reader.available()) {

				scoped_ptr<File> dataFile;

				// read line

				if(!reader.readLine(line,sizeof(line)))
					error("Failed to read line from file");

				// search for the = separator and break the text line at it

				if((ptr=strchr(line,'='))==nullptr)
					error("Badly formatted index.txt line - cannot find = symbol");

				*ptr='\0';

				// ensure this file can be opened

				if(!_fs->openFile(ptr,dataFile.address()))
					error("Cannot open data file");

				FlashEntry fe;
				fe.filename=strdup(line);
				fe.offset=atoi(ptr+1);
				fe.length=dataFile->getLength();

				_flashEntries.push_back(fe);

				*_usartStream << "Parsed " << fe.filename
						          << " offset " << fe.offset
						          << ", length " << fe.length
						          << "\r\n";
			}

			*_usartStream << "Finished reading index, "
										<< (uint32_t)_flashEntries.size() << ", entries read\r\n";
		}


		/*
		 * Unrecoverable error
		 */

		void error(const char *text) {
			status(text);
			for(;;);
		}


		/*
		 * Write a status string to the usart
		 */

		void status(const char *text) {
			*_usartStream << text << "\r\n";
		}
};


/*
 * Main entry point
 */

int main() {

	MillisecondTimer::initialise();

	FlashSpiProgram test;
	test.run();

	// not reached
	return 0;
}
