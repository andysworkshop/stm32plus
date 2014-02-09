/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


using namespace stm32plus;


/**
 * Timer demonstration: This demo uses the advanced control timer Timer1 to do something
 * not very advanced at all. We set the timer to count up and down, basically it oscillates
 * between 0 and 5000 at 1Khz. Each time one of the limits is hit the system raises an
 * 'Update' interrupt. We handle this interrupt by toggling a LED on PF6. The net result
 * of this is that the LED flashes on and off for a second in each of those states.
 *
 * If you are running this on the STM32F4DISCOVERY board then you can replace all reference
 * to PF6 with PD13 to use the onboard LED on that board.
 *
 * If you are running this on the VL or F0 DISCOVERY boards then you can replace all reference
 * to PF6 with PC8 to use the onboard LED on that board.
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

class TimerInterruptsTest  {

  protected:

    enum { LED_PIN = 6 };

    /*
     * We'll need these as member variables so that we can see them from the interrupt handler
     */

    GpioF<DefaultDigitalOutputFeature<LED_PIN> > _pf;
    bool _ledState;

  public:

    void run() {

      /*
       * Initialise timer1 running from the high speed internal APB2 (APB on the F0)
       * clock with an interrupt feature
       */

      Timer1<
        Timer1InternalClockFeature,       // the timer clock source is APB2 (APB on the F0)
        Timer1InterruptFeature            // gain access to interrupt functionality
      > timer;

      /*
       * We've got a LED on PF[6] and it's active LOW. Set it up here and switch it off.
       */

      _pf[LED_PIN].set();
      _ledState=false;

      /*
       * Set ourselves up as an observer for interrupts raised by the timer class.
       */

      timer.TimerInterruptEventSender.insertSubscriber(
          TimerInterruptEventSourceSlot::bind(this,&TimerInterruptsTest::onInterrupt)
        );


      /*
       * Set an up-down-timer up to tick at 5000Hz with an auto-reload value of 5000
       * The timer will count from 0 to 5000 inclusive, raise an Update interrupt and
       * then go backwards back down to 0 where it'll raise another Update interrupt
       * and start again. Each journey from one end to the other takes 1 second.
       *
       * Note that the lowest frequency you can set is 1098 for a 72Mhz timer clock source.
       * This is because the maximum prescaler value is 65536 (72Mhz/65536 = 1098Hz).
       */

      timer.setTimeBaseByFrequency(5000,4999,TIM_CounterMode_CenterAligned3);

      /*
       * Enable just the Update interrupt.
       */

      timer.enableInterrupts(TIM_IT_Update);

      /*
       * Start the timer
       */

      timer.enablePeripheral();

      /*
       * It's all running automatically now
       */

      for(;;);
    }

    /*
     * Observer callback function. This is called when the update interrupt that we've
     * enabled is fired.
     */

    void onInterrupt(TimerEventType tet,uint8_t /* timerNumber */) {

      if(tet==TimerEventType::EVENT_UPDATE) {

        /*
         * Toggle the LED state.
         */

        _ledState^=true;
        _pf[LED_PIN].setState(_ledState);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  TimerInterruptsTest test;
  test.run();

  // not reached
  return 0;
}
