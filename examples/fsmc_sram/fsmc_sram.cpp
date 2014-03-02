/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/sram.h"
#include "config/timing.h"
#include "config/stream.h"


using namespace stm32plus;


/**
 * Demo of an SRAM chip connected to the FSMC on an STM32F103ZET6 (LQFP144).
 * In this case it's an ISSI IS61LV25616 256K x 16 (4Mbit) SRAM on bank #3.
 *
 * This demo will write a repeating pattern to the SRAM and read it back
 * afterwards. If it is successful then a LED attached to PF6 will be
 * flashed for 500ms.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 */

class FsmcSramTest  {

  protected:

    /**
     * The pin number of the LED
     */

    enum { LED_PIN = 6 };

    /**
     * The SRAM and FSMC location typedef
     */

    typedef IS61LV25616<FsmcBank1NorSram3> MySram;

  public:

    void run() {

      GpioF<DefaultDigitalOutputFeature<LED_PIN> > pf;

      // initialise the SRAM

      MySram sram(MySram::TEN_NS);

      // lights off (this LED is active low, i.e. PF6 is a sink)

      pf[6].set();

      for(;;) {
        testDirect(pf[LED_PIN]);
        testStream(sram,pf[LED_PIN]);
      }
    }


    /**
     * Test by directly accessing the memory locations
     */

    void testDirect(GpioPinRef led) {

      uint32_t i;
      uint16_t *ptr;

      // write a pattern

      ptr=FsmcBank1NorSram3::getBaseAddress<uint16_t>();
      for(i=0;i<MySram::SIZE_IN_BYTES/2;i++)
        *ptr++=0xaa55;

      // read it back

      ptr=FsmcBank1NorSram3::getBaseAddress<uint16_t>();
      for(i=0;i<MySram::SIZE_IN_BYTES/2;i++)
        if(*ptr++!=0xaa55)
          for(;;);                  // lock up

      // switch the LED on and off for 500ms

      led.reset();
      MillisecondTimer::delay(500);
      led.set();
      MillisecondTimer::delay(500);
    }


    /**
     * Test by accessing as a stream. This test writes out a string repeatedly
     * until the SRAM is full and then reads it back to make sure it's good.
     */

    void testStream(MySram& sram,GpioPinRef led) {

      int32_t available;
      uint32_t actuallyRead;
      char buffer[28];

      {
        // initialise a buffered output stream on to the memory

        BlockDeviceOutputStream os(sram,0,true);

        // write out a pattern to the stream

        for(available=MySram::SIZE_IN_BYTES;available>=27;available-=27)
          os.write("Hello world, this is a test",27);     // 27 bytes
      }

      // initialise an input stream on to the memory

      BlockDeviceInputStream is(sram,0);

      // read back the data

      buffer[27]='\0';

      for(available=MySram::SIZE_IN_BYTES;available>=27;available-=27) {

        // get 27 bytes from the stream

        if(!is.read(buffer,27,actuallyRead) || actuallyRead!=27)
          for(;;);

        // ensure it's what we expect

        if(strcmp(buffer,"Hello world, this is a test")!=0)
          for(;;);
      }

      // switch the LED on and off for 500ms

      led.reset();
      MillisecondTimer::delay(500);
      led.set();
      MillisecondTimer::delay(500);
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  FsmcSramTest test;
  test.run();

  // not reached
  return 0;
}
