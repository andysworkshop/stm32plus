/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/sdcard.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * SD card demonstration using the onboard SDIO peripheral in high-speed 4-bit mode.
 * This example will erase 100 blocks from 20000 to 20099, then write a bit-pattern to
 * each of these blocks and finally read back each block to verify the bit-pattern.
 * Blocks are always 512 bytes in size, even if your card is a non-SDHC card that has
 * physical block sizes that are not 512 bytes.
 *
 * If all is OK then a LED on PF[6] will be flashed for 1-second at the end of each of
 * the erase, write, read sequences. If anything goes wrong then the LED will flash
 * rapidly at 5Hz.
 *
 * The SdioDmaSdCard class encapsulates access to the SDIO peripheral classes and uses
 * DMA and interrupts to transfer data. The default constructor will automatically detect
 * appropriate SDIO initialisation and transfer frequencies based on targets of 400kHz and
 * 24MHz, respectively. You can tune these dividers by using an SdioDmaSdCard constructor
 * parameter of 'false' (see the code comments below).
 *
 * Please note that this demo is destructive to any files that happen to be on your SD card
 * and are unfortunate enough to be located on the blocks we target. You should reformat the
 * card after you're done.
 *
 * Wiring:
 *   PC12 => SDIO_CLK
 *   PC8  => SDIO_D0
 *   PC9  => SDIO_D1
 *   PC10 => SDIO_D2
 *   PC11 => SDIO_D3
 *   PD2  => SDIO_CMD
 *
 * stm32plus enables the internal pullups on the data and command lines so you do not need
 * to connect external pullup resistors.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6
 *   STM32F103VET6
 *   STM32F103ZET6
 */

class SdioTest {

  protected:

    GpioF<DefaultDigitalOutputFeature<6> > _pf;
    SdioDmaSdCard *_sdcard;

  public:

      void run() {

        /*
         * Clear the LED. Our LED on PF[6] is active LOW.
         */

        _pf[6].set();


        /*
         * Initialise the sdcard with the SDIO interface feature. The constructor will attempt
         * to detect the initialisation and transfer clock dividers automatically for target frequencies
         * of 200kHz and 24MHz, respectively. You can override these by calling setInitDivider() and
         * setTransferDivider() and use 'false' as the constructor parameter so that the auto-init
         * does not happen. In this case you will need to call powerOn() and initialiseCard() yourself.
         */

        _sdcard=new SdioDmaSdCard;

        /*
         * Check for any errors raised in the constructor
         */

        if(errorProvider.hasError())
          error();

        for(;;) {

          eraseBlocks();

          writeSingleBlocks();
          readSingleBlocks();

          writeMultiBlocks();
          readMultiBlocks();

          // wait 5 seconds and do it again. it's not a good idea to go into a tight
          // program/erase loop with flash memory because it can wear out

          MillisecondTimer::delay(5000);
        }
      }



      /*
       * Erase blocks 20000 to 20099
       */

      void eraseBlocks() {

        // erase the blocks

        if(!_sdcard->eraseBlocks(20000,20099))
          error();

        // it worked

        signalOK();
      }


      /*
       * Write blocks 20000 to 20099 using the single block write command
       */

      void writeSingleBlocks() {

        int i;
        uint8_t block[512];

        // init the block

        for(i=0;i<512;i++)
          block[i]=i & 0xff;

        for(i=20000;i<20099;i++)
          if(!_sdcard->writeBlock(block,i))
            error();

        // it worked

        signalOK();
      }


      /*
       * Write blocks 20000 to 20099 using the multi-block write command
       * to write 20 blocks at a time
       */

      void writeMultiBlocks() {

        uint32_t i;
        uint8_t block[512*20];

        // init the blocks

        for(i=0;i<512*20;i++)
          block[i]=i & 0xff;

        for(i=20000;i<20099;i+=20)
          if(!_sdcard->writeBlocks(block,i,20))
            error();

        // it worked

        signalOK();
      }


      /*
       * Read blocks 20000 to 20099 using the single block read command
       */

      void readSingleBlocks() {

        int i,j;
        uint8_t block[512];

        // read each block

        for(i=20000;i<20099;i++) {

          // read this block

          if(!_sdcard->readBlock(block,i))
            error();

          // verify the content

          for(j=0;j<512;j++)
            if(block[j]!=(j & 0xff))
              error();
        }

        // it worked

        signalOK();
      }


      /*
       * Read blocks 20000 to 20099 using the multi-block read command
       * to read 20 blocks at a time
       */

      void readMultiBlocks() {

        uint32_t i,j;
        uint8_t blocks[512*20];

        for(i=20000;i<20099;i+=20) {

          // read these blocks

          if(!_sdcard->readBlocks(blocks,i,20))
            error();

          // verify the content

          for(j=0;j<512*20;j++)
            if(blocks[j]!=(j & 0xff))
              error();
        }

        // it worked

        signalOK();
      }


      /*
       * Signal that we completed an operation OK
       */

      void signalOK() {
        _pf[6].reset();                     // on
        MillisecondTimer::delay(1000);      // wait a second
        _pf[6].set();                       // off
        MillisecondTimer::delay(1000);      // wait a second
      }


      /*
       * There's been an error. Lock up and flash the LED on PF6 rapidly
       */

      void error() {

        for(;;) {

          _pf[6].set();
          MillisecondTimer::delay(200);
          _pf[6].reset();
          MillisecondTimer::delay(200);
        }
      }
  };


/*
 * Main entry point
 */

int main() {

  // we're using timing

  MillisecondTimer::initialise();

  // configure Nvic

  Nvic::initialise();

  SdioTest sdio;
  sdio.run();

  // not reached
  return 0;
}
