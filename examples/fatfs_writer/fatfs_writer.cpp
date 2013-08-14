/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/nvic.h"
#include "config/sdcard.h"
#include "config/filesystem.h"
#include "config/usart.h"
#include "config/string.h"


using namespace stm32plus;


/**
 * FAT file system writer demo.
 *
 * This example will create a file called "test.txt" in a
 * directory called "output" on the SD card. If the
 * directory does not exist then it will be created. If
 * the file does exist then it will be deleted before we
 * open it for writing.
 *
 * When we're done writing to it we will re-open the
 * file and write the content to USART1.
 *
 * Note that if you are using the STM32F4DISCOVERY board
 * then you cannot use Usart1 since the pins clash with
 * onboard peripherals. I have tested this code on that
 * board using Usart2.
 *
 * The SD card must be inserted and ready when this
 * application runs. This demo was tested on a 4Gb
 * class 10 SDHC microSD card.
 *
 * Compatible MCU:
 * 	 STM32F1
 * 	 STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class FatFsWriterTest  {

	protected:

		// SD card and file system objects

		SdioDmaSdCard *_sdcard;
		FileSystem *_fs;
		NullTimeProvider _timeProvider;

		// USART objects

		typedef Usart1<> MyUsart;
		MyUsart *_usart;

	public:

		/*
		 * Run the demo
		 */

		void run() {

			// initialisation for the card and LCD

			initUsart();
			initSdcard();

			// now do the work

			ensureDirectoryExists();
			ensureFileDoesNotExist();
			writeToFile();
			sendFile();

			// we're done - lock up

			for(;;);
		}


		/*
		 * Ensure that /output exists. If it does not then it will be created. If it
		 * does exist then we check that it is a directory and not a file.
		 */

		void ensureDirectoryExists() {

			FileInformation *info;

			// get a FileInformation for the directory

			if(_fs->getFileInformation("/output",info)) {

				// it exists - but if it's a file then it's an error

				if((info->getAttributes() & FileInformation::ATTR_DIRECTORY)==0)
					error();

				// it's a directory, that's OK

				delete info;
				return;
			}

			// it does not exist, create it

			if(!_fs->createDirectory("/output"))
				error();
		}


		/*
		 * Ensure that the file does not exist. If it does exist then
		 * it will be deleted.
		 */

		void ensureFileDoesNotExist() {

			FileInformation *info;

			// get a FileInformation for the file

			if(_fs->getFileInformation("/output/test.txt",info)) {

				// it exists - make sure it's not a directory

				if((info->getAttributes() & FileInformation::ATTR_DIRECTORY)!=0)
					error();

				// clean up

				delete info;

				// we know it's a file, delete it

				if(!_fs->deleteFile("/output/test.txt"))
					error();

				return;
			}
		}


		/*
		 * Write the text "Hello World" to /output/test.txt. It is not necessary to delete/create
		 * the file - it could be re-opened and overwritten either from the beginning or by seeking
		 * to the end and appending to it.
		 */

		void writeToFile() {

			File *file;

			// any previous file has been erased, open this file. there is no concept of file mode
			// here - any file can be read or written to.

			if(!_fs->createFile("/output/test.txt"))
				error();

			// open the 0 byte file

			if(!_fs->openFile("/output/test.txt",file))
				error();

			// write some test data

			if(!file->write("Hello World",11))
				error();

			// finished with the file - close it

			delete file;
		}


		/*
		 * open the file and send the contents to the USART
		 */

		void sendFile() {

			File *file;

			// open the file - we own the file pointer that comes back upon success and
			// we must remember to delete it when we're finished

			if(!_fs->openFile("/output/test.txt",file))
				error();

			// attach an input stream to the file for easy sequential reading and an output
			// stream to the usart for writing

			FileInputStream input(*file);
			UsartPollingOutputStream output(*_usart);

			// the ConnectedInputOutputStream is a piece of plumbing that will copy one
			// stream to another either as a whole or in chunks

			ConnectedInputOutputStream connector(input,output);

			// copy the entire file to the USART

			if(!connector.readWrite())
				error();

			// finished with the file

			delete file;
		}


		/*
		 * Initialise the USART in 57600-8-N-1 mode with no hardware flow control
		 */

		void initUsart() {

			// configure the USART peripheral

			_usart=new MyUsart(57600);
		}


		/*
		 * Initialise the SD card and get a reference to a file system object. FAT16 and FAT32
		 * are both supported.
		 */

		void initSdcard() {

			// create the SDIO object and let it auto-initialise

			_sdcard=new SdioDmaSdCard;

			if(errorProvider.hasError())
				error();

			// initialise a file system from that found on the card

			if(!FileSystem::getInstance(*_sdcard,_timeProvider,_fs))
				error();
		}


		/*
		 * Print an error code if something goes wrong and lock up
		 */

		void error() {

			char *ptr,errorCode[30];

			// print the error code

			StringUtil::modp_uitoa10(errorProvider.getLast(),errorCode);

			for(ptr=errorCode;*ptr;ptr++)
				_usart->send(*ptr);

			// lock up

			for(;;);
		}
};


/*
 * Main entry point
 */

int main() {

	// set up the NVIC priority groups and subgroups
	Nvic::initialise();

	// set up SysTick at 1ms resolution
	MillisecondTimer::initialise();

	FatFsWriterTest test;
	test.run();

	// not reached
	return 0;
}
