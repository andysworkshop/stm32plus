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

    typedef InternalFlashDevice<InternalFlashWriteFeature> MyFlash;
    typedef InternalFlashSettingsStorage<Settings,MyFlash> MySettingsStorage;


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

      // erase the settings pages

      if(!storage.erase())
        error(1);

      // start the setting integer counter

      settingsOut.intValue=12345678;

      // 32 bytes per setting = 128 settings in two 1024 byte pages. To test we will write out
      // 130 times so we see a page boundary crossing and a reset back to the beginning

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

        if(settingsOut.intValue!=settingsIn.intValue || !strcmp(settingsOut.stringValue,settingsIn.stringValue))
          error(4);
      }

      // reset the internal state of the storage so it has to go looking for the settings again

      storage.reset();

      // read back

      memset(&settingsIn,'\0',sizeof(settingsIn));
      if(!storage.read(settingsIn))
        error(5);

      // check values

      if(settingsOut.intValue!=settingsIn.intValue || !strcmp(settingsOut.stringValue,settingsIn.stringValue))
        error(6);

      // finished OK

      error(0);
    }


    /*
     * Get the first location based on the device
     */

    constexpr uint32_t getFirstLocation() const {

#if defined(STM32PLUS_F0_51)
      return FLASH_BASE+65536-2048;      // 2Kb at the top of the 64Kb flash
#else
#error Unsupported MCU
#endif

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
