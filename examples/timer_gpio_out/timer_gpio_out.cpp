/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


using namespace stm32plus;


/**
 * Timer demonstration: Output a 1Hz toggle on/off signal on PA0.
 *
 * To achieve the alternating 1Hz signal on PA0 (TIM2_CH1) we first
 * set up Timer2 to tick at 10kHz with its auto-reload register set to 9999.
 * It will take 2 seconds for the timer to tick up to 9999 at which point it
 * will reset back to zero.
 *
 * We configure the channel 1 compare value to be 9999 with an action of
 * "toggle" and we enable GPIO output for channel 1 on its default port of PA0.
 *
 * To see the output, connect PA0 to a LED. e.g. on the F0 and F1 VL discovery
 * boards you can connect PA0 to PC8.
 *
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F051R8T6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class TimerGpioOutTest {

  public:

    void run() {

      /*
       * Initialise timer2 running from the internal APB2 clock with channel-1 and GPIO output features.
       * The GPIO output feature is itself configured with a channel-1 output feature.
       */

      Timer2<
        Timer2InternalClockFeature,       // the timer clock source is HCLK/2
        TimerChannel1Feature<TimerChannelOCPolarityHighFeature>,           // we're going to use channel 1
        Timer2GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM2_CH1_OUT                    // we will output channel 1 to GPIO
        >
      > timer;

      /*
       * Set an up-timer up to tick at 1000Hz with an auto-reload value of 9999
       * The timer will count from 0 to 9999 inclusive then reset back to 0.
       * It will take exactly 1 second to do this.
       *
       * Note that the lowest frequency on the F1 that you can set is 1098 for a
       * 72Mhz timer clock source. This is because the maximum prescaler value is
       * 65536 (72Mhz/65536 = 1098Hz).
       */

      timer.setTimeBaseByFrequency(10000,9999);

      /*
       * Initialise the channel 1 output compare value to 9999 with the default
       * action of toggle.
       */

      timer.initCompare(9999);

      /*
       * Start the timer. Each time it ticks up to 9999 (which will take 1 second)
       * the channel 1 compare value will be triggered and the TIM2_CH1 GPIO pin on PA0
       * will be toggled. Since we've hooked this up to a LED we should see it flashing
       * at 1Hz.
       */

      timer.enablePeripheral();

      /*
       * It's all running automatically now
       */

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  TimerGpioOutTest test;
  test.run();

  // not reached
  return 0;
}
