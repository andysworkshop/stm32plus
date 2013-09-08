/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F1)

#include "config/rtc.h"
#include "config/gpio.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Real time clock (RTC) demo.
 *
 * This demo sets up the RTC to flash a LED on PF6 every second. Additionally, an alarm is set
 * to go off every 10 seconds and when it does the LED is flashed rapidly 5 times.
 *
 * The RTC on the F1 is quite different to the F4 so I have elected to provide separate demos
 * for the F1 and F4 (STM32F4DISCOVERY)
 *
 * Compatible MCU:
 *   STM32F1
 *
 * Tested devices:
 *   STM32F103ZET6
 */

class RtcTest {

  protected:
    uint32_t _alarmTick;
    bool _ledState;

    volatile bool _ticked;
    volatile bool _alarmed;

  public:

    void run() {

      // initialise the LED port

      GpioF<DefaultDigitalOutputFeature<6> > pf;

      // lights off (this LED is active low, i.e. PF6 is a sink)

      _ledState=true;
      pf[6].set();

      // declare an RTC instance customised with just the features we will use.
      // a clock source is mandatory. The interrupt features are optional and
      // will pull in the relevant methods and features for us to use

      Rtc<
        RtcLseClockFeature,             // we'll clock it from the LSE clock
        RtcSecondInterruptFeature,      // we want per-second interrupts
        RtcAlarmInterruptFeature        // we also want the alarm interrupt
      > rtc;

      // insert ourselves as subscribers to the per-second and alarm interrupts.

      rtc.RtcSecondInterruptEventSender.insertSubscriber(RtcSecondInterruptEventSourceSlot::bind(this,&RtcTest::onTick));
      rtc.RtcAlarmInterruptEventSender.insertSubscriber(RtcAlarmInterruptEventSourceSlot::bind(this,&RtcTest::onAlarm));

      _ticked=_alarmed=false;

      // start the second interrupt

      rtc.enableSecondInterrupt();

      // configure the alarm to go off after 10 seconds

      rtc.setAlarm(_alarmTick=10);

      // main loop

      for(;;) {

        // if we ticked, toggle LED state

        if(_ticked) {
          _ledState^=true;
          pf[6].setState(_ledState);

          // reset for next time

          _ticked=false;
        }

        // if the alarm went off then flash rapidly

        if(_alarmed) {

          for(int i=0;i<5;i++) {
            pf[6].reset();
            MillisecondTimer::delay(50);
            pf[6].set();
            MillisecondTimer::delay(50);
          }

          // put the LED back where it was

          pf[6].setState(_ledState);

          // bump forward the alarm by 10 seconds

          _alarmTick+=10;
          rtc.setAlarm(_alarmTick);

          // reset for next time

          _alarmed=false;
        }
      }
    }


    /*
     * The RTC has ticked
     */

    void onTick() {
      _ticked=true;
    }


    /*
     * The RTC has alarmed
     */

    void onAlarm() {
      _alarmed=true;
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  RtcTest test;
  test.run();

  // not reached
  return 0;
}

#endif // STM32PLUS_F1
