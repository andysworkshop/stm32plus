/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/timer.h"
#include "string/StringUtil.h"


using namespace stm32plus;


/**
 * This example shows how to drive a seven segment, three digit common-cathode
 * LED display using resistors and MOSFETs as the only external components.
 * Timer 1 is used to generate an interrupt at a frequency of 180Hz. Each
 * interrupt sets up the GPIOs to drive one of the digits with the other two
 * switched off. Therefore the update rate for each digit is 180/3 = 60Hz.
 * This is enough to avoid visual flickering.
 *
 * The wiring required is:
 *   68 ohm resistors between each GPIO port and the LED segment pins. Connect
 *   each digit pin to the drain of an n-channel MOSFET. Connect PB9, PB2, PB10
 *   to the corresponding MOSFET gates. Ground all 3 MOSFET source pins.
 *
 *     PA0: segment A
 *     PA3: segment B
 *     PB8: segment C
 *     PB4: segment D
 *     PB3: segment E
 *     PA1: segment F
 *     PA2: segment G
 *     PB5: decimal point segment
 *     PB9: digit 1 MOSFET gate
 *     PB2: digit 2 MOSFET gate
 *     PB10: digit 3 MOSFET gate
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F042F6P6
 *   STM32F051R8T6
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */


/*
 * Abbreviated table of digits to switched on for each character. The table is
 * in ASCII order starting at 32 for space. Each bit is a segment in order ABCDEFG0.
 */

static const uint8_t AsciiTable[]= {
  0,  // SPACE
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // skip
  0b11111100,  // 0
  0b01100000,  // 1
  0b11011010,  // 2
  0b11110010,  // 3
  0b01100110,  // 4
  0b10110110,  // 5
  0b10111110,  // 6
  0b11100000,  // 7
  0b11111110,  // 8
  0b11110110   // 9
};


class Timer7SegmentTest  {

  protected:

    enum {
      SEGA = 0,   // PA0
      SEGB = 3,   // PA3
      SEGC = 8,   // PB8
      SEGD = 4,   // PB4
      SEGE = 3,   // PB3
      SEGF = 1,   // PA1
      SEGG = 2,   // PA2
      SEGP = 5,   // PB5
      DIG1 = 9,   // PB9
      DIG2 = 2,   // PB2
      DIG3 = 10   // PB10
    };

    /*
     * Segments A-G, DP in that order
     */

    GpioPinRef _segments[8];

    /*
     * Digits 1,2,3
     */

    GpioPinRef _digits[3];

    /*
     * State variables. Copy the ASCII string to display to _display
     * and it will show up immediately.
     */

    volatile char _display[7];   // 3 digits, each can be followed by '.'
    volatile uint8_t _currentDigit;
    volatile char *_currentDigitPtr;

  public:

    void run() {

      /*
       * Initialise the GPIO pins. The SEGx pins will drive HIGH to light a
       * segment and on the other side of the LED DIGx will be set HIGH to
       * switch on the MOSFET that permits the digit to light up.
       */

      GpioA<DefaultDigitalOutputFeature<SEGA,SEGB,SEGF,SEGG>> pa;
      GpioB<DefaultDigitalOutputFeature<DIG1,DIG2,DIG3,SEGC,SEGD,SEGE,SEGP>> pb;

      // set up the state variables

      _segments[0]=pa[SEGA];
      _segments[1]=pa[SEGB];
      _segments[2]=pb[SEGC];
      _segments[3]=pb[SEGD];
      _segments[4]=pb[SEGE];
      _segments[5]=pa[SEGF];
      _segments[6]=pa[SEGG];
      _segments[7]=pb[SEGP];

      _digits[0]=pb[DIG1];
      _digits[1]=pb[DIG2];
      _digits[2]=pb[DIG3];

      /*
       * Turn all the LEDs off by setting everything to low
       */

      for(uint8_t i=0;i<sizeof(_segments)/sizeof(_segments[0]);i++)
        _segments[0].reset();

      for(uint8_t i=0;i<sizeof(_digits)/sizeof(_digits[0]);i++)
        _digits[0].reset();

      /*
       * Initialise the state variables
       */

      _display[0]=_display[1]=_display[2]=0;
      _currentDigit=0;
      _currentDigitPtr=_display;

      /*
       * Initialise timer1 running from the high speed internal APB2 (APB on the F0)
       * clock with an interrupt feature
       */

      Timer1<
        Timer1InternalClockFeature,       // the timer clock source is APB2 (APB on the F0)
        Timer1InterruptFeature            // gain access to interrupt functionality
      > timer;

      /*
       * Set ourselves up as a subscriber for interrupts raised by the timer class.
       */

      timer.TimerInterruptEventSender.insertSubscriber(
          TimerInterruptEventSourceSlot::bind(this,&Timer7SegmentTest::onInterrupt)
        );


      /*
       * Set an up-down-timer up to tick at 80kHz with an auto-reload value of 444
       * The timer will count from 0 to 444 inclusive, raise an Update interrupt and
       * then go backwards back down to 0 where it'll raise another Update interrupt
       * and start again. Each journey from one end to the other takes 1/180 second.
       */

      timer.setTimeBaseByFrequency(80000,444,TIM_CounterMode_CenterAligned3);

      /*
       * Enable just the Update interrupt, clearing any spurious pending flag first
       */

      timer.clearPendingInterruptsFlag(TIM_IT_Update);
      timer.enableInterrupts(TIM_IT_Update);

      /*
       * Start the timer
       */

      timer.enablePeripheral();

      /*
       * We'll display an upward counter that wraps at 999 and shows each number
       * for 100ms
       */

      int value = -1;

      for(;;) {

        value++;

        if(value>999)
          value = 0;

        // translate value to ascii, left justified

        _display[0]=_display[1]=_display[2]=0;
        StringUtil::itoa(value, const_cast<char *>( _display), 10);

        // wait for 100ms

        MillisecondTimer::delay(100);
      }
    }

    /*
     * Subscriber callback function. This is called when the update interrupt that we've
     * enabled is fired.
     */

    void onInterrupt(TimerEventType tet,uint8_t /* timerNumber */) {

      // verify our expectation

      if(tet!=TimerEventType::EVENT_UPDATE)
        return;

      // turn off the last digit we displayed. This needs to be done first to avoid
      // switched off segments becoming faintly visible during multiplexing

      _digits[_currentDigit].reset();

      // advance to the digit we just set up

      if(_currentDigit>=2) {
        _currentDigit=0;
        _currentDigitPtr=_display;
      }
      else
        _currentDigit++;

      // get the character to display at this position

      uint8_t c=*_currentDigitPtr++;

      // check the bottom end of the range

      if(c<=' ')
        c=' ';

      // get the segment state bitmap from the table

      uint8_t bits=AsciiTable[c-' '];

      // for each bit in the map, set/reset the correct state in the segments

      for(uint8_t j=0;j<7;j++) {
        bool state=(bits & 0x80)!=0;
        _segments[j].setState(state);
        bits <<= 1;
      }

      // process the decimal point if there is one

      if(*_currentDigitPtr=='.') {
        _segments[7].set();
        _currentDigitPtr++;
      }
      else
        _segments[7].reset();

      // switch on the digit we have set up

      _digits[_currentDigit].set();

      // we'll be back in 1/180s which means we are displaying each digit at 60Hz
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

  Timer7SegmentTest test;
  test.run();

  // not reached
  return 0;
}
