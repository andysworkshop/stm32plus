/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/timer.h"
#include "config/usart.h"
#include "config/string.h"


using namespace stm32plus;


/**
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

class TimerEncoderTest {

  protected:

    uint16_t _lastValue;
    bool _moved;

    enum {
      LED1_PIN = 12,
      LED2_PIN = 13
    };

  public:

    void run() {

      _lastValue=0;
      _moved=false;

      GpioD<DefaultDigitalOutputFeature<LED1_PIN,LED2_PIN>> leds;

      Timer1<
         Timer1GpioFeature<
           TIMER_REMAP_NONE,               // the GPIO output will not be remapped
           TIM1_CH1_IN,
           TIM2_CH1_IN                     // we will output channel 1 to GPIO
         >,
         TimerEncoderFeature<
           EncoderCounterEdge::Input1,
           EncoderPolarity::Rising,
           EncoderPolarity::Rising
         >,
         TimerChannel1Feature,
         Timer1InterruptFeature
      > timer;

      Usart3_Remap1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /*
       * Set ourselves up as a subscriber for interrupts raised by the timer class.
       */

      timer.TimerInterruptEventSender.insertSubscriber(
          TimerInterruptEventSourceSlot::bind(this,&TimerEncoderTest::onInterrupt)
        );

      /*
       */

      timer.initialiseUpCounter(5);

      timer.enableInterrupts(TIM_IT_CC1);

      timer.initCapture();

      /*
       */

      timer.enablePeripheral();

      _lastValue=timer.getCounter();

      uint16_t currentValue;

      for(;;) {

        do {
          currentValue=timer.getCounter();
        } while(currentValue==_lastValue);

        if(currentValue>_lastValue) {
          leds[LED1_PIN].set();
          MillisecondTimer::delay(200);
          leds[LED1_PIN].reset();
        }
        else if(currentValue<_lastValue) {
          leds[LED2_PIN].set();
          MillisecondTimer::delay(200);
          leds[LED2_PIN].reset();
        }

        outputStream << StringUtil::Ascii(currentValue) << "\r\n";

        _lastValue=currentValue;
        _moved=false;
      }




      /*
      for(;;) {

        while(!_moved);

        uint16_t currentValue;

        currentValue=timer.getCounter();

        if(currentValue>_lastValue) {
          leds[LED1_PIN].set();
          MillisecondTimer::delay(200);
          leds[LED1_PIN].reset();
        }
        else if(currentValue<_lastValue) {
          leds[LED2_PIN].set();
          MillisecondTimer::delay(200);
          leds[LED2_PIN].reset();
        }

        outputStream << StringUtil::Ascii(currentValue) << "\r\n";

        _lastValue=currentValue;
        _moved=false;
      }
      */
    }


    /*
     * Subscriber callback function. This is called when the CC1 interrupt that we've
     * enabled is fired.
     */

    void onInterrupt(TimerEventType tet,uint8_t /* timerNumber */) {

      if(tet==TimerEventType::EVENT_COMPARE1)
        _moved=true;
    }
};


/*
 * Main entry point
 */

int main() {

  MillisecondTimer::initialise();

  TimerEncoderTest test;
  test.run();

  // not reached
  return 0;
}
