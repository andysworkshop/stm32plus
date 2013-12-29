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
 * Timer demonstration: Use timer 2 to output a 1Mhz PWM wave on channel 1 (PA0) and channel 2 (PA1).
 * The demo will repeatedly increment the duty cycle on each channel from 0 to 100% and back down to
 * 0 over 800ms. Channel 1 fades up while channel 2 fades down.
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

class TimerDualPwmGpioOutTest {

  public:

    void run() {

      /*
       * Initialise timer2 running from the internal APB1 (APB on the F0) clock with
       * channel-1, channel-2 and GPIO output features. The GPIO output feature is
       * itself configured with channel-1 and channel-2 output features.
       */

      Timer2<
        Timer2InternalClockFeature,       // the timer clock source is APB1 (APB on the F0)
        TimerChannel1Feature,             // we're going to use channel 1...
        TimerChannel2Feature,             // ...and we're going to use channel 2
        Timer2GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM2_CH1_OUT,                   // we will output channel 2 to GPIO...
          TIM2_CH2_OUT                    // ...and we will output channel 2 to GPIO
        >
      > timer;

      /*
       * Set an up-timer up to tick at 10MHz with an auto-reload value of 1999
       * The timer will count from 0 to 1999 inclusive then reset back to 0.
       *
       * Note that the lowest frequency you can set is 1098 for a 72Mhz timer clock source.
       * This is because the maximum prescaler value is 65536 (72Mhz/65536 = 1098Hz).
       */

      timer.setTimeBaseByFrequency(10000000,1999);

      /*
       * Initialise channel 1 as a PWM channel in edge-aligned mode (TIM_OCMode_PWM1).
       * The default starting duty cycle is zero. Note that we have to qualify the initCompare
       * call because we're including multiple channels in our timer class and their members
       * have the same names.
       */

      timer.TimerChannel1Feature::initCompareForPwmOutput();

      /*
       * Initialise channel 2 as a PWM channel in edge-aligned mode (TIM_OCMode_PWM1).
       * The default starting duty cycle is zero.
       */

      timer.TimerChannel2Feature::initCompareForPwmOutput();

      /*
       * Enable the timer. The PWM outputs are on PA0 and PA1.
       */

      timer.enablePeripheral();

      /*
       * It's all running automatically now, use the main CPU to vary the duty cycle up
       * to 100% and back down again
       */

      for(;;) {

        // fade channel 1 up to 100% in 4ms steps while channel 2 comes down to 0

        for(int8_t i=0;i<=100;i++) {
          timer.TimerChannel1Feature::setDutyCycle(i);
          timer.TimerChannel2Feature::setDutyCycle(100-i);
          MillisecondTimer::delay(4);
        }

        // fade channel 1 down to 0% in 4ms steps while channel 2 goes up to 100%

        for(int8_t i=100;i>=0;i--) {
          timer.TimerChannel1Feature::setDutyCycle(i);
          timer.TimerChannel2Feature::setDutyCycle(100-i);
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

  TimerDualPwmGpioOutTest test;
  test.run();

  // not reached
  return 0;
}
