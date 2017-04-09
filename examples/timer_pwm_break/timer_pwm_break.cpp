/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Timer demonstration including the break input feature: Use timer 1 to output a 1MHz PWM wave on
 * channel 1 (PA8). The demo will repeatedly increment the duty cycle from 0 to 100% and back down
 * to 0 over 800ms. When the break input is triggered (low to high) the PWM output will stop and
 * be reset to the programmed idle state (low).
 *
 * Connect up PA8 to a LED pin on your development board to see the output. Connect up the break input
 * (PB12) to GND when you launch the demo.
 *
 * To trigger the break input remove the connection from PB12 to GND. This alone will probably be enough
 * to trigger the break. If not then connect it to the board's VCC voltage (probably 3V or 3.3V). The
 * break interrupt will repeatedly trigger until you reconnect PB12 to ground.
 *
 * It's up to you to recover from the break in the interrupt handler before re-enabling the outputs. If
 * you want the outputs to be automatically re-enabled at the next reload event then call enableAutomaticOutput()
 * during the timer setup.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F407VGT6
 *   STM32F051R8T6
 *   STM32F103VET6
 */

class TimerPwmBreakTest {

  public:

    void run() {

      /*
       * Initialise timer1 running from the internal APB2 clock with channel-1 and GPIO output features.
       * The GPIO output feature is itself configured with a channel-1 output feature.
       */

      Timer1<
        Timer1InternalClockFeature,       // the timer clock source is APB1
        UnlockedTimerBreakFeature,        // we want the break functionality
        TimerChannel1Feature<             // we're going to use channel 1 with the OC idle state = low
          TimerChannelOCPWM1ModeFeature,
          TimerChannelOCHighPolarityFeature,
          TimerChannelOCLowIdleFeature
        >,
        Timer1GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM1_CH1_OUT,                   // we will output channel 1 to GPIO
          TIM1_BKIN                       // break input on PB12
        >,
        Timer1InterruptFeature
      > timer;


      /*
       * Set ourselves up as a subscriber for interrupts raised by the timer class.
       */

      timer.TimerInterruptEventSender.insertSubscriber(
          TimerInterruptEventSourceSlot::bind(this,&TimerPwmBreakTest::onInterrupt)
        );

      /*
       * Set an up-timer up to tick at 10MHz with an auto-reload value of 1999
       * The timer will count from 0 to 1999 inclusive then reset back to 0.
       */

      timer.setTimeBaseByFrequency(10000000,1999);

      /*
       * Clear and enable interrupts
       */

      timer.clearPendingInterruptsFlag(TIM_IT_Break);
      timer.enableInterrupts(TIM_IT_Break);

      /*
       * Default duty cycle of zero
       */

      timer.setDutyCycle(0);

      /*
       * Master output enable (timers with the break feature have this disabled after reset)
       */

      timer.enableMainOutput();

      /*
       * Enable the break feature
       */

      timer.setBreakPolarity(UnlockedTimerBreakFeature::BREAK_POLARITY_HIGH);
      timer.enableBreak();

      /*
       * Enable the timer. The PWM output is on PA8.
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


    /*
     * Subscriber callback function
     */

    void onInterrupt(TimerEventType tet,uint8_t /* timerNumber */) {

      if(tet==TimerEventType::EVENT_BREAK) {
        // do something to handle the break event
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // we need the SysTick timer

  MillisecondTimer::initialise();

  TimerPwmBreakTest test;
  test.run();

  // not reached
  return 0;
}
