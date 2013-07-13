/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/rtc.h"
#include "config/gpio.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Real time clock (RTC) demo.
 *
 * This demo sets up the RTC to flash a LED on PD13 every second. Additionally, an alarm is set
 * to go off every 10 seconds and when it does the LED is flashed rapidly 5 times. The clock source
 * is the LSI assumed to be 32768Hz.
 *
 * The RTC on the F1 is quite different to the F4 so I have elected to provide separate demos
 * for the F1 and F4 (STM32F4DISCOVERY). This is the F4 demo.
 *
 * Compatible MCU:
 * 	 STM32F4
 *
 * Tested devices:
 *   STM32F407VGT6
 */

class RtcTest {

	protected:

		enum { LED_PIN = 13 };

		bool _ledState;

		volatile bool _ticked;
		volatile bool _alarmed;

	public:

		void run() {

			// initialise the LED port

			GpioD<DefaultDigitalOutputFeature<LED_PIN> > pd;

			// lights off (this LED is active high, i.e. PD13 is a source)

			_ledState=true;
			pd[LED_PIN].reset();

			// declare an RTC instance customised with just the features we will use.
			// a clock source is mandatory. The interrupt features are optional and
			// will pull in the relevant methods and features for us to use

			Rtc<
				RtcLsiClockFeature<RtcMeasuredLsiFrequencyProvider>,	// we'll clock it from the LSI clock and calibrate the LSI using a timer
				RtcSecondInterruptFeature,														// we want per-second interrupts
				RtcAlarmAInterruptFeature															// we also want the alarm A interrupt
			> rtc;

			// insert ourselves as subscribers to the per-second and alarm interrupts.
			// we need to qualify ExtiInterruptSender with the name of its containing class because
			// there are two ExtiInterruptSender's in the hierarchy.

			rtc.RtcSecondInterruptFeature::ExtiInterruptEventSender.insertSubscriber(ExtiInterruptEventSourceSlot::bind(this,&RtcTest::onTick));
			rtc.RtcAlarmInterruptFeature::ExtiInterruptEventSender.insertSubscriber(ExtiInterruptEventSourceSlot::bind(this,&RtcTest::onAlarm));

			// set the time to midnight

			rtc.setTime(0,0,0);

			_ticked=_alarmed=false;

			// start the second interrupt

			rtc.enableSecondInterrupt();

			// configure the alarm to go off on 10s time match. i.e. 0:0:10, 0:1:10, 0:2:10 etc...

			rtc.setAlarm(RTC_AlarmMask_DateWeekDay | RTC_AlarmMask_Hours | RTC_AlarmMask_Minutes,		// only consider seconds as the trigger
									 RTC_AlarmDateWeekDaySel_Date,			// don't care
									 0,																	// day/date (don't care)
									 0,																	// hour (don't care)
									 0,																	// minute (don't care)
									 10);																// second - on the 10's.

			// main loop

			for(;;) {

				// if we ticked, toggle LED state

				if(_ticked) {
					_ledState^=true;
					pd[LED_PIN].setState(_ledState);

					// reset for next time

					_ticked=false;
				}

				// if the alarm went off then flash rapidly

				if(_alarmed) {

					for(int i=0;i<5;i++) {
						pd[LED_PIN].set();
						MillisecondTimer::delay(50);
						pd[LED_PIN].reset();
						MillisecondTimer::delay(50);
					}

					// put the LED back where it was

					pd[LED_PIN].setState(_ledState);

					// reset for next time (in another 60 seconds)

					_alarmed=false;
				}
			}
		}

		/*
		 * the RTC has ticked
		 */

		void onTick(uint8_t /* extiNumber */) {
			_ticked=true;
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

#endif // STM32PLUS_F4
