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
 *   STM32F0
 *
 * Tested on devices:
 *   STM32F051R8T6
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

#if defined(STM32PLUS_F0)
    typedef InternalFlashDevice<InternalFlashWriteFeature> MyFlash;
#elif defined(STM32PLUS_F4)
    typedef InternalFlashDevice<InternalFlashSectorMapF40xF41x,DefaultF407InternalFlashWriteFeature> MyFlash;
#endif

    typedef InternalFlashSettingsStorage<Settings,MyFlash> MySettingsStorage;


    /*
     * Get the first location based on the device
     */

    constexpr uint32_t getFirstLocation() const {

#if defined(STM32PLUS_F0_51) || defined(STM32PLUS_F0_30)

      return FLASH_BASE+65536-2048;      // 2Kb at the top of the 64Kb flash

#elif defined(STM32PLUS_F407)

      return FLASH_BASE+100000-(128*1024);   // 128Kb (last page) at the top of the flash

#else
#error Unsupported MCU
#endif

    }


    /*
     * Run the test
     */

    void run() {

      // create the objects. We'll use 2 pages for the settings storage

      MySettingsStorage::Parameters params(getFirstLocation(),2);
      MyFlash flash;
      MySettingsStorage storage(flash,params);
      Settings settingsOut,settingsIn;
      uint8_t i;

      InternalFlashWordWriter<MyFlash> w(flash,0);

      // erase the settings pages

      if(!storage.erase())
        error(1);

      // start the setting integer counter

      settingsOut.intValue=12345678;

      // 32 bytes per setting = 64 settings in two 1024 byte pages. To test we will write out
      // 67 times so we see a page boundary crossing and a reset back to the beginning where
      // we'll write out a few more times

      for(i=0;i<67;i++) {

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
