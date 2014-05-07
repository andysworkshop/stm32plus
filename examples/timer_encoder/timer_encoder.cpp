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
 * This timer example demonstrates the use of the encoder feature. The STM32's timers understand
 * the phase relationship between two signals output by a rotary encoder and can translate those
 * rising and falling edges into up/down movements of a counter. All you need to do is to connect
 * up the two encoder output pins to channels 1 and 2 of a timer and then configure the timer to
 * operate in encoder mode. The timer's counter will then tick up and down as the encoder is operated.
 *
 * We will use timer 1 to receive the encoder inputs. We will subscribe to capture/compare interrupts
 * so that we get notified when the encoder is moved rather than having to poll for it. We will light
 * different LEDs depending on the direction that the encoder is turned and we will output the value
 * of the counter to the USART.
 *
 * Some wiring is obviously required.
 *
 * If you have an encoder then wire the outputs to PA8, PA9 and remove the call to simulateSignals()
 * in the code. simulateSignals() allows you to validate your code before attaching your hardware.
 * To run in simulation mode you must wire PA0 => PA8 and PA1 => PA9. The simulator will pretend to
 * be an encoder and stimulate the encoder TI1 and TI2 inputs according to a known pattern.
 *
 * F4 Discovery board:
 *   This example will work out-of-the-box on the F4 discovery board. LEDs are on PD12, PD13. The
 *   USART TX/RX is PD8/PD9.
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

    /*
     * LED pins on port D
     */

    enum {
      LED1_PIN = 12,
      LED2_PIN = 13
    };

    /*
     * Set up the two LEDs that will show the direction of the counter (up/down)
     */

    GpioD<DefaultDigitalOutputFeature<LED1_PIN,LED2_PIN>> _leds;

    /*
     * Set up the timer that will be used to receive input from the encoder
     */

    Timer1<
       Timer1GpioFeature<               // TI1/TI2 are CH1_IN and CH2_IN, set them up
         TIMER_REMAP_NONE,              // the GPIO input will not be remapped
         TIM1_CH1_IN,                   // channel 1 = TI1
         TIM2_CH1_IN                    // channel 2 = TI2
       >,
       TimerEncoderFeature<             // we're using the quadrature encoder feature
         EncoderCounterEdge::Input1,    // count on TI1 edges with direction according to TI2
         EncoderPolarity::Rising,       // TI1 polarity
         EncoderPolarity::Rising        // TI2 polarity
       >,
       TimerChannel1Feature,            // we'll use channel 1 for CC interrupts
       Timer1InterruptFeature           // that means we need the interrupt feature
    > _timer;

    /*
     * Declare a USART to write out the counter value. TX/RX = PD8/PD9 for this one.
     */

    Usart3_Remap1<> _usart;
    UsartPollingOutputStream _outputStream;

    /*
     * The last value received from the counter
     */

    uint16_t _lastValue;

  public:

    /*
     * Constructor, call the member variable constructors where needed
     */

    TimerEncoderTest() :
      _usart(57600),
      _outputStream(_usart) {
    }


    /*
     * Run the test
     */

    void run() {

      /*
       * Set ourselves up as a subscriber for interrupts raised by the timer class.
       */

      _timer.TimerInterruptEventSender.insertSubscriber(
          TimerInterruptEventSourceSlot::bind(this,&TimerEncoderTest::onInterrupt)
        );

      /*
       * Initialise the counter for the encoder to have a range of 0..65535 and
       * an initial value of 32767
       */

      _timer.initialiseEncoderCounter(65535,32767);
      _lastValue=32767;

      /*
       * Initialise the capture and subscribe to the interrupts
       */

      _timer.initCapture();
      _timer.enableInterrupts(TIM_IT_CC1);


      /*
       * Enable the timer
       */

      _timer.enablePeripheral();

#if 0
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
      }
#endif

      /*
       * Go into an infinite loop simulating the encoder signals. If you've got a real
       * encoder then delete the simulateSignals() and delay() calls and just go into
       * an infinite loop.
       */

      for(;;) {
        simulateSignals();
        MillisecondTimer::delay(1000);
      }
    }


    /*
     * Simulate the signals that an encoder might make. This requires you to make the following
     * wiring: PA0 => PA8. PA1 => PA9. TI1 and TI2 will be generated on PA0/PA1 following the
     * pattern in AN4013 Figure 8. This will simulate 4 up counts and then 4 down counts.
     */

    void simulateSignals() {

      static const bool ti[][2]={
          { false, false },
          { true,  false },         // +1
          { true,  true },
          { false, true },          // +1
          { false, false },
          { true,  false },         // +1
          { true,  true },
          { false, true },          // +1
          { false, false },
          { true,  false },         // reversal
          { false, false },
          { true,  false },
          { false, false },         // -1
          { false, true },
          { true,  true },          // -1
          { true,  false },
          { false, false },         // -1
          { false, true },
          { true,  true },          // -1
          { true,  false },
          { false, false },
          { false, true },          // reversal
          { false, false },
          { false, true },
          { false, false },
          { false, true },
          { false, false }
      };

      enum { TI1 = 0, TI2 = 1 };
      GpioA<DefaultDigitalOutputFeature<TI1,TI2> > simpins;

      uint8_t i;

      // run through the steps allowing 10ms between each one

      for(i=0;i<sizeof(ti)/sizeof(ti[0]);i++) {

        simpins[TI1].setState(ti[i][0]);
        simpins[TI2].setState(ti[i][1]);

        MillisecondTimer::delay(10);
      }
    }


    /*
     * Subscriber callback function. This is called when the CC1 interrupt that we've
     * enabled is fired.
     */

    void onInterrupt(TimerEventType tet,uint8_t /* timerNumber */) {

      uint16_t currentValue;

      /*
       * Not likely to be anything else but anyway...
       */

      if(tet!=TimerEventType::EVENT_COMPARE1)
        return;

      /*
       * A positive change will light LED1, a negative change will light LED2
       */

      currentValue=_timer.getCounter();
      if(currentValue>_lastValue || _lastValue-currentValue>30000) {

        // current > last or there's been a wrap (i.e. silly difference between last and current)

        _leds[LED1_PIN].set();
        _leds[LED2_PIN].reset();
      }
      else {
        _leds[LED1_PIN].reset();
        _leds[LED2_PIN].set();
      }

      /*
       * Write out the current value to the USART if it's connected
       */

      _outputStream << StringUtil::Ascii(currentValue) << "\r\n";
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
