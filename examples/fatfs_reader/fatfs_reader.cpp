/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/nvic.h"
#include "config/sdcard.h"
#include "config/filesystem.h"
#include "config/usart.h"
#include "config/string.h"
#include "config/smartptr.h"


using namespace stm32plus;


/**
 * FAT file system reader demo.
 *
 * This example will open a text file from a subdirectory of the SD card and write
 * the contents to USART1. The filename is /files/test.txt. To run this demo you must
 * have USART1 wired to a terminal program on the PC that is set up to receive at
 * 57600-8-N-1 mode. I recommend the free "RealTerm" program.
 *
 * Note that if you are using the STM32F4DISCOVERY board then you cannot use Usart1
 * since the pins clash with onboard peripherals. I have tested this code on that
 * board using Usart2.
 *
 * The SD card must be inserted and ready when this application runs. This demo was
 * tested on a 4Gb class 10 SDHC microSD card.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class FatFsReaderTest  {

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

      // now send the file

      sendFile("/files/test.txt");

      // we're done - lock up

      for(;;);
    }


    /*
     * open the file and send the contents to the USART
     */

    void sendFile(const char *filename) {

      scoped_ptr<File> file;
      char buffer[256];

      // open the file - we own the file pointer that comes back upon success and
      // we must remember to delete it when we're finished

      if(!_fs->openFile(filename,file.address()))
        error();

      // declare a file reader to read lines from the file and also a
      // UsartPollingOutputStream for convenient output to the USART

      FileReader reader(*file);
      UsartPollingOutputStream output(*_usart);

      while(reader.available()) {

        // read the next line and send to the USART

        if(!reader.readLine(buffer,sizeof(buffer)))
          error();

        output << buffer << "\r\n";
      }
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

  FatFsReaderTest test;
  test.run();

  // not reached
  return 0;
}
