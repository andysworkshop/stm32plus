/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/button.h"


using namespace stm32plus;


/**
 * Auto-repeating button demo that doesn't use interrupts
 *
 * This demo assumes that you have a button on PA0 and an
 * LED on PD13. Hold down the button and the LED will
 * flash to indicate the presses. The default
 * configuration of PA0, PD13 and a LOW state when
 * pressed is correct for the STM32F4DISCOVERY board.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class Button {

  protected:

    static const bool BUTTON_PRESSED_HIGH  = false;   // false = LOW when pressed, true = HIGH when pressed
    static const uint32_t INITIAL_DELAY_MS = 600;
    static const uint32_t REPEAT_DELAY_MS  = 150;

  public:
    void run() {

      // initialise the ports

      GpioD<DefaultDigitalOutputFeature<13> > pd;
      GpioA<DefaultDigitalInputFeature<0> > pa;

      // lights off (this LED is active high, i.e. PD13 is a source)

      pd[13].reset();

      // create the button class with parameters

      AutoRepeatPushButton button(pa[0],BUTTON_PRESSED_HIGH,INITIAL_DELAY_MS,REPEAT_DELAY_MS);

      // main loop

      for(;;) {

        // sample the button and swith the LED on (HIGH) or off (LOW)

        if(button.getState()==PushButton::Pressed) {

          // switch the LED on for 10ms

          pd[13].set();
          MillisecondTimer::delay(10);
        }
        else
          pd[13].reset();
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  Button button;
  button.run();

  // not reached
  return 0;
}
