/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Timer demonstration: Use timer 2 to output a 1MHz PWM
 * wave on channel 1 (PA0). The demo will repeatedly
 * increment the duty cycle from 0 to 100% and back down
 * to 0 over 800ms.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class TimerPwmGpioOutTest {

  public:

    void run() {

      /*
       * Initialise timer2 running from the internal APB2 clock with channel-1 and GPIO output features.
       * The GPIO output feature is itself configured with a channel-1 output feature.
       */

      Timer2<
        Timer2InternalClockFeature,       // the timer clock source is APB1
        TimerChannel1Feature,             // we're going to use channel 1
        Timer2GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM2_CH1_OUT                    // we will output channel 1 to GPIO
        >
      > timer;

      /*
       * Set an up-timer up to tick at 10MHz with an auto-reload value of 1999
       * The timer will count from 0 to 1999 inclusive then reset back to 0.
       *
       * Note that the lowest frequency you can set on the F1 is 1098 for a 72MHz
       * timer clock source. This is because the maximum prescaler value is 65536
       * (72Mhz/65536 = 1098Hz).
       */

      timer.setTimeBaseByFrequency(10000000,1999);

      /*
       * Initialise channel 1 as a PWM channel in edge-aligned mode (TIM_OCMode_PWM1).
       * The default starting duty cycle is zero.
       */

      timer.initCompareForPwmOutput();

      /*
       * Enable the timer. The PWM output is on PA0.
       */

      timer.enablePeripheral();

      /*
       * It's all running automatically now, use the main CPU to vary the duty cycle up
       * to 100% and back down again
       */

      for(;;) {

        // fade up to 100% in 4ms steps

        for(int8_t i=0;i<=100;i++) {
          timer.setDutyCycle(i);
          MillisecondTimer::delay(4);
        }

        // fade down to 0% in 4ms steps

        for(int8_t i=100;i>=0;i--) {
          timer.setDutyCycle(i);
          MillisecondTimer::delay(4);
        }
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // we need the SysTick timer

  MillisecondTimer::initialise();

  TimerPwmGpioOutTest test;
  test.run();

  // not reached
  return 0;
}
