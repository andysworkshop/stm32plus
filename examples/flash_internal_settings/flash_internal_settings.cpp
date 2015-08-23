/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "config/flash/internal.h"
#include "config/string.h"


using namespace stm32plus;


/**
 * This example demonstrates saving your application configuration settings to the internal
 * flash pages of the MCU with a wear levelling strategy to ensure that your flash memory
 * does not wear out prematurely. See the comments in the InternalFlashSettingsStorage
 * template for a detailed explanation of how this works.
 *
 * Unfortunately ST have chosen to implement the internal flash with differing page sizes
 * even within the same MCU family so this example will contain preprocessor ugliness to select
 * the correct page size and flash location for each device.
 *
 * IMPORTANT: CHECK THAT THE FLASH START ADDRESS IS CORRECT FOR YOUR MCU. THE INTENTION IS
 * TO USE THE LAST PAGES IN THE DEVICES MEMORY.
 *
 * This example will exercise the code by writing the settings enough times that it will
 * wrap around back to the first page. If it works then the LED on PC8 will light solid. If there's
 * a problem then the LED will flash a number of times to indicate a numeric error code that
 * you can cross reference with the code.
 *
 * Compatible MCU:
 *   STM32F051
 *   STM32F407
 *   STM32F103
 *
 * Tested on devices:
 *   STM32F051R8T6
 *   STM32F407VGT6
 *   STM32F103ZET6
 */

class InternalFlashSettings {

  protected:

    /*
     * The default LED pin is PC8. Change this and the port declaration in error() if your board has
     * it somewhere else
     */

    enum {
      LED_PIN = 8
    };


  public:

    /*
     * Example settings class. We'll store a string and an integer (total 14 bytes). The storage
     * required will be 14+8 rounded up to power of 2 = 32 bytes.
     */

    struct Settings {
      char stringValue[10];
      uint32_t intValue;
    };

    // set up the types needed for the storage

#if defined(STM32PLUS_F0) || defined(STM32PLUS_F1)
    typedef InternalFlashDevice<InternalFlashWriteFeature> MyFlash;
#elif defined(STM32PLUS_F4)
    typedef InternalFlashDevice<InternalFlashSectorMapF40xF41x,DefaultF407InternalFlashWriteFeature> MyFlash;
#endif

    typedef InternalFlashSettingsStorage<Settings,MyFlash> MySettingsStorage;


    /*
     * Get the first location based on the device
     */

    void getFlashParameters(uint32_t& firstLocation,uint32_t& memorySize) const {

#if defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)

      firstLocation=FLASH_BASE+65536-2048;           // 2Kb at the top of the 64Kb flash
      memorySize=2048;

#elif defined(STM32PLUS_F407)

      firstLocation=FLASH_BASE+1048576-(128*1024);   // 128Kb (last page) at the top of the 1Mb flash
      memorySize=128*1024;

#elif defined(STM32PLUS_F1_HD)

      firstLocation=FLASH_BASE+524288-4096;          // 4Kb (2x2Kb pages) at the top of 512Kb flash
      memorySize=4096;

#else
#error Unsupported MCU
#endif
    }


    /*
     * Run the test
     */

    void run() {

      // create the objects. We'll use 2 pages for the settings storage

      MySettingsStorage::Parameters params;
      getFlashParameters(params.firstLocation,params.memorySize);

      MyFlash flash;
      MySettingsStorage storage(flash,params);
      Settings settingsOut,settingsIn;
      uint8_t i;

      // erase the settings pages

      if(!storage.erase())
        error(1);

      // start the setting integer counter

      settingsOut.intValue=12345678;

      // 130 writes will cause a wrap-around on the F0 and F1.

      for(i=0;i<130;i++) {

        // increment the setting

        settingsOut.intValue++;

        // convert the integer counter to string

        StringUtil::modp_uitoa10(settingsOut.intValue,settingsOut.stringValue);

        // attempt to write to flash

        if(!storage.write(settingsOut))
          error(2);

        // attempt to read back

        if(!storage.read(settingsIn))
          error(3);

        // check that the values match

        if(settingsOut.intValue!=settingsIn.intValue || strcmp(settingsOut.stringValue,settingsIn.stringValue))
          error(4);
      }

      // reset the internal state of the storage so it has to go looking for the settings again

      storage.reset();

      // read back

      memset(&settingsIn,'\0',sizeof(settingsIn));
      if(!storage.read(settingsIn))
        error(5);

      // check values

      if(settingsOut.intValue!=settingsIn.intValue || strcmp(settingsOut.stringValue,settingsIn.stringValue))
        error(6);

      // finished OK

      error(0);
    }


    /*
     * An error occurred, light the LED constantly
     */

    void error(uint8_t flashes) {

      GpioC<DefaultDigitalOutputFeature<LED_PIN> > led;
      uint8_t i;

      if(flashes==0) {

        // success case: just light solid

        led[LED_PIN].set();
        for(;;);
      }

      for(;;) {

        for(i=0;i<flashes;i++) {

          led[LED_PIN].set();
          MillisecondTimer::delay(250);
          led[LED_PIN].reset();
          MillisecondTimer::delay(250);
        }

        // 3 second delay after the last flash

        MillisecondTimer::delay(2750);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  InternalFlashSettings ifs;
  ifs.run();

  // not reached
  return 0;
}
