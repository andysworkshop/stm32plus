/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/exti.h"
#include "config/power.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Low power modes test. This example requires a
 * pushbutton wired to PA8 and an indicator LED wired
 * to PF6.
 *
 * The LED will continually flash as 250ms intervals
 * until the button is pressed at which point the MCU
 * will go into STOP mode pending an interrupt to wake
 * it up. Press the button again will cause the EXTI
 * interrupt that will wake it up.
 *
 * The commented out line _lpm.standby() can be
 * substituted for the stopInterruptWakeup() call to
 * cause the MCU to enter STANDBY mode. STANDBY can be
 * exited by WAKEUP, IWDG, RTC alarm and of course
 * external reset. STANDBY mode clears SRAM therefore
 * execution after wakeup is back at the start of your
 * program (the reset handler).
 *
 * To run this example on the STM32F4DISCOVERY board
 * change the LED pin to PD13 to use the onboard LED
 * and change the button pin to PA0 to use the onboard
 * user button. Also change Exti8 to Exti0 to match
 * the button pin number.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class PowerTest {

  protected:

    enum {
      BUTTON_PIN = 8,
      LED_PIN = 6
    };

    LowPowerModes _lpm;
    bool _triggered;

  public:

    void run() {

      // initialise the LED and button pins

      GpioF<DefaultDigitalOutputFeature<LED_PIN> > pf;
      GpioA<DefaultDigitalInputFeature<BUTTON_PIN> > pa;

      // subscribe to interrupts from the button

      Exti8 exti(EXTI_Mode_Interrupt,EXTI_Trigger_Falling,pa[BUTTON_PIN]);

      exti.ExtiInterruptEventSender.insertSubscriber(
          ExtiInterruptEventSourceSlot::bind(this,&PowerTest::onInterrupt)
        );

      // if we're here because of a previous standby then flash quickly 3 times

      if(LowPowerModes::wasInStandby())
        flashLed(pf[LED_PIN],3);

      // if we're here because of a wakeup then flash quickly 6 times

      if(LowPowerModes::wasWokenUp())
        flashLed(pf[LED_PIN],6);

      // enable the WAKEUP pin

      _lpm.enableWakeup();

      _triggered=false;

      for(;;) {

        // flash the LED

        pf[LED_PIN].set();
        MillisecondTimer::delay(250);
        pf[LED_PIN].reset();
        MillisecondTimer::delay(250);

        // if the interrupt set the trigger, do our stuff

        if(_triggered) {

          // wakeup following STOP will continue after here
          _lpm.stopInterruptWakeup();

          // wakeup following STANDBY will reset the device
          //_lpm.standby();

          _triggered=false;
        }
      }
    }

    /*
     * Observer notification callback for the EXTI interrupt that's wired to the falling
     * edge of the button GPIO line.
     */

    void onInterrupt(uint8_t /* extiLine */) {
      _triggered=true;
    }


    /**
     * Flash the LED for the number times then wait 3 seconds
     * @param count number of times to flash
     */

    void flashLed(const GpioPinRef& gpio,int count) {

      for(int i=0;i<count;i++) {
        gpio.reset();
        MillisecondTimer::delay(100);
        gpio.set();
        MillisecondTimer::delay(100);
      }

      MillisecondTimer::delay(3000);
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  PowerTest test;
  test.run();

  // not reached
  return 0;
}
