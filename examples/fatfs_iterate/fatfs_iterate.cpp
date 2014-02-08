/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/nvic.h"
#include "config/sdcard.h"
#include "config/filesystem.h"
#include "config/display/tft.h"

// we're going to use the STL string

#include <iterator>
#include <string>


using namespace stm32plus;
using namespace stm32plus::display;


/**
 * FAT file system iterator demo.
 *
 * This example will recursively iterate over the
 * directories and files on a FAT16/32 file system
 * on an SD card connected via SDIO. The SD card
 * must be inserted and ready when this application
 * runs. This demo was tested on a 4Gb class 10
 * SDHC microSD card.
 *
 * The output of the program is sent to a graphical
 * LCD, in this case an ILI9325 panel connected via
 * the FSMC. Other LCD drivers can be used by
 * changing the device driver name and ensuring that
 * the correct access mode is selected (8 or 16 bit).
 *
 * Because I use the STL string I have added
 * include/stl to the compile-time include path.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class FatFsIterateTest  {

  protected:

    // graphics library and terminal types and objects

    typedef Fsmc16BitAccessMode<FsmcBank1NorSram1> LcdAccessMode;
    typedef ILI9325_Portrait_64K<LcdAccessMode> LcdPanel;
    typedef ILI9325_Terminal_Portrait<LcdPanel> LcdTerminal;

    LcdAccessMode *_accessMode;
    LcdPanel *_graphicsLibrary;
    LcdTerminal *_terminal;
    Font *_font;
    DefaultBacklight *_backlight;

    // SD card and file system objects

    SdioDmaSdCard *_sdcard;
    FileSystem *_fs;
    NullTimeProvider _timeProvider;

  public:

    /*
     * Run the demo
     */

    void run() {

      // initialisation for the card and LCD

      initLcd();
      initSdcard();

      // now iterate the directories on the card

      iterateDirectories("");

      // we're done - lock up

      *_terminal << "Completed";
      for(;;);
    }


    /*
     * Recursively iterate over the directories and files on this card
     */

    void iterateDirectories(const std::string& directoryName) {

      DirectoryIterator *it;

      // print the name of the current directory

      *_terminal << "=> " << directoryName.c_str() << "\n";

      // get an iterator on to this directory. note that we own the returned
      // pointer and must delete it when we're finished.

      if(!_fs->getDirectoryIterator(directoryName.c_str(),it))
        error();

      // iterate over all entries in this directory

      while(it->next()) {

        // get a reference to the FileInformation object that describes the file/directory
        // that we're currently looking at

        const FileInformation& fileInfo(it->current());

        // print the name and length in bytes. other attributes such as the type and various
        // date/times are also available

        *_terminal << fileInfo.getFilename() << " = " << fileInfo.getLength() << " bytes\n";

        // if this is a directory and it's not one of the two special "." and ".." entries then
        // recursively print its contents

        if((fileInfo.getAttributes() & FileInformation::ATTR_DIRECTORY)!=0
            && strcmp(".",fileInfo.getFilename())!=0
            && strcmp("..",fileInfo.getFilename())!=0) {

          iterateDirectories(directoryName+"/"+std::string(fileInfo.getFilename()));
        }
      }

      // finished with the iterator, delete it

      delete it;
    }


    /*
     * Initialise the LCD. This demo uses the ILI9325 QVGA LCD connected to the FSMC
     * in 16 bit mode. We use a portrait-orientation terminal that will take advantage of
     * the hardware scrolling ability of the panel.
     */

    void initLcd() {

      // reset is on PE1 and RS (D/CX) is on PD11

      GpioE<DefaultDigitalOutputFeature<1> > pe;
      GpioD<DefaultFsmcAlternateFunctionFeature<11>> pd;

      // set up the FSMC with RS=A16 (PD11)

      Fsmc8080LcdTiming fsmcTiming(0,2);
      _accessMode=new LcdAccessMode(fsmcTiming,16,pe[1]);

      // declare an lcd driver and make a fixed font active

      _graphicsLibrary=new LcdPanel(*_accessMode);
      _font=new Font_APPLE8;
      *_graphicsLibrary << *_font;

      // declare the terminal

      _terminal=new LcdTerminal(*_graphicsLibrary);

      // clear the screen

      _graphicsLibrary->setBackground(ColourNames::BLACK);
      _graphicsLibrary->setForeground(ColourNames::WHITE);

      _terminal->clearScreen();

      // create the backlight on timer4, channel2 (PD13)

      DefaultBacklight backlight;

      // fade up to 100% in 4ms steps

      backlight.fadeTo(100,4);
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

      // print the error code

      *_terminal << "ERROR: " << errorProvider.getLast();

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

  FatFsIterateTest test;
  test.run();

  // not reached
  return 0;
}
