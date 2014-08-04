/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


using namespace stm32plus;


/**
 * Timer demonstration: Output a 1Hz toggle on/off signal on PA0 and another 1Hz
 * signal 500ms ahead of the first one, output on PA1.
 *
 * The timer is configured to tick at 4000Hz with an auto-reload value of 3999.
 * Channel 1 is configured to output an alternating signal on PA0 when the counter
 * reaches 3999. Similarly, channel 2 is configured to output an alternating signal
 * on PA1 when the counter reaches 1000.
 *
 * We configure the channel 1 compare value to be 3999 with an action of "toggle"
 * and we enable GPIO output for channel 1 on its default port of PA0.
 *
 * To see the output, connect PA0 and PA1 to LEDs. e.g. on the F0 and F1 VL discovery
 * boards you can connect PA0 => PC8 and PA1 => PC9.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F051R8T6
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class TimerDualGpioOutTest {

  public:

    void run() {

      /*
       * Initialise timer2 running from the internal APB1 (APB on the F0) clock with channel-1,
       * channel-2 and GPIO output features. The GPIO output feature is itself configured with
       * channel-1 and channel-2 output features.
       */

      Timer2<
        Timer2InternalClockFeature,       // the timer clock source is APB1 (APB on the F0)
        TimerChannel1Feature<>,           // we're going to use channel 1...
        TimerChannel2Feature<>,           // ...and we're going to use channel 2
        Timer2GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM2_CH1_OUT,                   // we will output channel 1 to GPIO PA0
          TIM2_CH2_OUT                    // ...and also channel 2 to GPIO PA1
        >
      > timer;

      /*
       * Set an up-timer up to tick at 4000Hz with an auto-reload value of 3999
       * The timer will count from 0 to 3999 inclusive then reset back to 0.
       * It will take exactly 1 second to do this.
       *
       * Note that the lowest frequency you can set is 1098 for a 72Mhz timer clock source.
       * This is because the maximum prescaler value is 65536 (72Mhz/65536 = 1098Hz).
       */

      timer.setTimeBaseByFrequency(4000,3999);

      /*
       * Initialise the channel 1 output compare value to 3999 with the default
       * action of toggle. Note that we have to qualify the initCompare call because
       * we're including multiple channels in our timer class and their members have
       * the same names.
       */

      timer.TimerChannel1Feature<>::initCompare(3999);

      /*
       * Initialise the channel 2 output compare value to 2000 with the default
       * action of toggle. The toggle frequency is still 1Hz but it will be out
       * of phase with the toggling of channel 1.
       */

      timer.TimerChannel2Feature<>::initCompare(2000);

      /*
       * Start the timer. Each time it ticks up to 4000 (which will take 1 second)
       * the channel 1 compare value will be triggered and the TIM1_CH1 GPIO pin on PA0
       * will be toggled. Since we've hooked this up to a LED we should see it flashing
       * at 1Hz.
       *
       * When it hits 1000 then the same goes for TIM2_CH2 GPIO on PA1.
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

  TimerDualGpioOutTest test;
  test.run();

  // not reached
  return 0;
}
