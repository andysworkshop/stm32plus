/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F0)

#include "config/rtc.h"
#include "config/gpio.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Real time clock (RTC) demo.
 *
 * This demo sets up the RTC to flash a LED on PC8 rapidly every 5 seconds. The clock source is the
 * HSI (8MHz) oscillator that we will calibrate using timer 14.
 *
 * The RTC on the F0 is quite different to the F1 but is similar to the F4 though it lacks the per-second
 * interrupt that you get with the F4 peripheral and you only get one alarm.
 *
 * Compatible MCU:
 *   STM32F0
 *
 * Tested devices:
 *   STM32F042F6P6
 *   STM32F051R8T6
 */

class RtcTest {

  protected:

    enum { LED_PIN = 8 };

    volatile bool _alarmed;

  public:

    void run() {

      // initialise the LED port

      GpioC<DefaultDigitalOutputFeature<LED_PIN> > pc;

      // lights off (this LED is active high, i.e. PC8 is a source)

      pc[LED_PIN].reset();

      // declare an RTC instance customised with just the features we will use.
      // a clock source is mandatory. The interrupt feature is optional and
      // will pull in the relevant methods and features for us to use

      Rtc<
        RtcLsiClockFeature<RtcMeasuredLsiFrequencyProvider>,  // we'll clock it from the LSI clock and calibrate the HSI using a timer
        RtcAlarmInterruptFeature                              // we also want the alarm A interrupt
      > rtc;

      // insert ourselves as a subscriber to the alarm interrupt

      rtc.RtcAlarmInterruptFeature::ExtiInterruptEventSender.insertSubscriber(ExtiInterruptEventSourceSlot::bind(this,&RtcTest::onAlarm));

      // set the time to midnight

      rtc.setTime(0,0,0);

      _alarmed=false;

      // configure the alarm to go off on 10s time match. i.e. 0:0:10, 0:1:10, 0:2:10 etc...

      rtc.setAlarm(RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes,   // only consider seconds as the trigger
                   RTC_AlarmDateWeekDaySel_Date,      // don't care
                   0,                                 // day/date (don't care)
                   0,                                 // hour (don't care)
                   0,                                 // minute (don't care)
                   10);                               // second - on the 10's.

      // main loop

      for(;;) {

        // if the alarm went off then flash rapidly

        if(_alarmed) {

          for(int i=0;i<5;i++) {
            pc[LED_PIN].set();
            MillisecondTimer::delay(50);
            pc[LED_PIN].reset();
            MillisecondTimer::delay(50);
          }

          // reset for next time (in another 10 seconds)

          rtc.setTime(0,0,0);
          _alarmed=false;
        }
      }
    }

    /*
     * the RTC has alarmed
     */

    void onAlarm(uint8_t /* extiNumber */) {
      _alarmed=true;
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution

  MillisecondTimer::initialise();

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  RtcTest test;
  test.run();

  // not reached
  return 0;
}

#endif // STM32PLUS_F0
