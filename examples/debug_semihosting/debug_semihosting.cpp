/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

/*
 * Enable Semihosting support. This is the only change you need to make to enable
 * and disable semihosting. There is no need to surround your SemiHosting output
 * commands with #ifdef guards because a 'null' SemiHosting class is provided for
 * normal builds that will be optimised to nothing by the optimser. A real project
 * will define this in the project options (make, eclipse, scons etc).
 */

#define SEMIHOSTING


#include "config/stm32plus.h"
#include "config/gpio.h"
#include "config/timing.h"
#include "config/debug.h"


using namespace stm32plus;


/**
 * Semihosting is the useful ability to send debug strings to the host's console using the
 * STLink SWD debugger protocol. To use it with stm32plus ensure that you define SEMIHOSTING
 * (see above) and ensure that your OpenOCD debugger startup options include:
 *
 *   monitor arm semihosting enable
 *
 * If you fail to include the above line then you'll hit a breakpoint in the semihosting
 * implementation code at runtime.
 *
 * This example adds semihosting capabilities to the 'blink' example. It is the most basic
 * example that uses GPIO and the Systick timer to blink a LED on PC8 at 1Hz. PC8 is wired
 * to the blue onboard LED on the F1VL and F0 discovery boards.
 *
 * If the STM32F4DISCOVERY board is your target then change LED_PIN to 13 and the GPIO
 * declation to...
 *
 *  GpioD<DefaultDigitalOutputFeature<LED_PIN> > pd
 *
 * ... and change 2 of "pd[LED_PIN]" to "pd[LED_PIN]" to blink the orange led on the
 * discovery board.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F030R8T6
 *   STM32F042F6P6
 *   STM32F051R8T6
 *   STM32F100RBT6
 *   STM32F407VGT6
 */

class SemiHostingTest {

  public:

    enum {
      LED_PIN = 8
    };

    void run() {

      // initialise the pin for output

      GpioC<DefaultDigitalOutputFeature<LED_PIN> > pc;

      // loop forever switching it on and off with a 1 second
      // delay in between each cycle

      for(int i=0;;i++) {

        // show the current iteration status in the OpenOCD terminal window

        shost << "Blink loop, iteration " << i << "\n";

        pc[LED_PIN].set();
        MillisecondTimer::delay(1000);

        pc[LED_PIN].reset();
        MillisecondTimer::delay(1000);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  shost.puts("stm32plus blink is starting\n");

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  SemiHostingTest demo;
  demo.run();

  // not reached
  return 0;
}
