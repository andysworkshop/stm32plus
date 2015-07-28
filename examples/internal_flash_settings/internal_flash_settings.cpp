/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "config/flash/internal.h"


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

  public:

    /*
     * Example settings class. We'll store a string and an integer (total 14 bytes)
     */

    struct Settings {
      char stringSetting[10];
      uint32_t intSetting;
    };

    void run() {

      // set up the types needed for the storage

      typedef InternalFlashDevice<InternalFlashWriteFeature> MyFlash;
      typedef InternalFlashSettingsStorage<Settings,MyFlash> MySettingsStorage;

      // create the objects. We'll use 2 pages for the settings storage

      MySettingsStorage::Parameters params(getFirstLocation(),2);
      MyFlash flash;
      MySettingsStorage storage(flash,params);
      Settings settings;

      // create first settings

      strcpy(settings.stringSetting,"12345678");
      settings.intSetting=0x01020304;

      if(!storage.write(settings))
        error();
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

    void error() {

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
