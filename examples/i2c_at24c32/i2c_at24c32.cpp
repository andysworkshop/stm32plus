/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/i2c.h"
#include "config/eeprom.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * The AT24C32 is a 32768kbit (4096kbyte) EEPROM with
 * an I2C serial interface.
 *
 * This test assumes that you've got it connected to
 * I2C#2 with the default pins. These pins are:
 *
 * PB10 => I2C2_SCL
 * PB11 => I2C2_SDA
 *
 * The test will repeatedly write and then read back a
 * pattern from the EEPROM. If it all works then a LED
 * on PF6 will flash for one second. If there is a
 * failure then the LED will flash rapidly forever.
 *
 * To run this example on the STM32F4DISCOVERY board
 * change the LED pin to PD13 to use the onboard LED.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class AT24C32Test {

  protected:

    enum {
      LED_PIN = 6
    };

  public:

    void run() {

      /*
       * Define a type for the EEPROM
       */

      typedef AT24C32<
        I2C2_Default<I2CTwoByteMasterPollingFeature>    // the EEPROM has 2-byte addresses
      > MyEeprom;

      uint16_t address;
      uint32_t actuallyRead;
      uint8_t i,c,buffer[237];

      /*
       * initialise the LED and switch if off (this LED is
       * active LOW
       */

      GpioF<DefaultDigitalOutputFeature<LED_PIN> > pf;
      pf[LED_PIN].set();

      /*
       * Initialise the AT24C32 on I2C #2. We will be the bus master
       * and we will poll it
       */

      I2C::Parameters params;
      MyEeprom eeprom(params);

      /*
       * Fill a buffer with a simple test pattern
       */

      for(i=0;i<sizeof(buffer);i++)
        buffer[i]=i;

      /*
       * Test forever
       */

      for(;;) {

        /*
         * Reset to position zero and write a byte. Note that the SerialEeprom
         * base class inherits from InputStream and OutputStream so you can use
         * the overloaded << and >> operators to write to and read from the EEPROM
         */

        eeprom.seek(0);

        if(!eeprom.writeByte(0xaa))
          error(pf[LED_PIN]);

        /*
         * Let the device settle after write (10ms max, see datasheet)
         */

        MillisecondTimer::delay(10);

        /*
         * Read back the byte and check it
         */

        eeprom.seek(0);
        c=0;

        if(!eeprom.readByte(c) || c!=0xaa)
          error(pf[LED_PIN]);

        /*
         * Write the 237 byte sequence at a random position
         */

        address=rand() % (MyEeprom::SIZE_IN_BYTES-sizeof(buffer));

        eeprom.seek(address);
        if(!eeprom.write(buffer,sizeof(buffer)))
          error(pf[LED_PIN]);

        /*
         * Let the device settle after write (10ms max, see datasheet)
         */

        MillisecondTimer::delay(10);

        /*
         * Clear the buffer and read back the data
         */

        memset(buffer,0,sizeof(buffer));

        eeprom.seek(address);
        if(!eeprom.read(buffer,sizeof(buffer),actuallyRead))
          error(pf[LED_PIN]);

        for(i=0;i<sizeof(buffer);i++)
          if(buffer[i]!=i)
            error(pf[LED_PIN]);

        /*
         * Success, flash for a second
         */

        pf[LED_PIN].reset();
        MillisecondTimer::delay(1000);
        pf[LED_PIN].set();
        MillisecondTimer::delay(1000);
      }
    }


    /*
     * Error, flash rapidly on and off
     */

    void error(const GpioPinRef& gpio) {
      for(;;) {
        gpio.reset();
        MillisecondTimer::delay(100);
        gpio.set();
        MillisecondTimer::delay(100);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  AT24C32Test test;
  test.run();

  // not reached
  return 0;
}
