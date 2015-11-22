/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"
#include "config/dma.h"


using namespace stm32plus;


/**
 * Timer demo that illustrates how to use a DMA channel to automatically reload
 * the PWM duty cycle.
 *
 * In this example timer 1 is configured to output a PWM signal on channel 2.
 * The timer's DMA channel for update events is used to automatically reload the
 * PWM duty cycle from a sequence that we supply each time the timer gets an
 * update event.
 *
 * The PWM signal is output on PA[9]. Connect this to a LED to see the fade feature.
 * On the F1VL and F4 discovery boards this means connecting PA9 to PC8
 * or PD13, respectively. The F0 discovery board is also PC8.
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

class TimerDmaPwmTest {

  public:

    void run() {

      uint8_t i,percents[200];

      /*
       * Create an instance of Timer1 running off the internal clock
       * with channel 1 and unremapped GPIO output features.
       */

      Timer1<
        Timer1InternalClockFeature,       // the timer bus is APB2
        TimerChannel2Feature<>,           // we're going to use channel 2
        Timer1GpioFeature<                // we want to output something to GPIO
          TIMER_REMAP_NONE,               // the GPIO output will not be remapped
          TIM1_CH2_OUT                    // we will output channel 2 to GPIO
        >
      > timer;

      /*
       * Create an instance of the DMA channel that is connected to
       * Timer1's update event and add in a PWM fader feature for
       * Timer1's channel 2. This will be a circular DMA configuration, i.e.
       * it will automatically run itself over and over again until we
       * stop it.
       */

      Timer1UpdateDmaChannel<
        Timer1Channel2UpdatePwmFadeTimerDmaFeature<DMA_Priority_High,DMA_Mode_Circular>
      > dma;

      // create a sequence of 0..100 (101 values)

      for(i=0;i<=100;i++)
        percents[i]=i;

      // follow it with a sequence of 99..1 (99 values)

      for(i=99;i>0;i--)
        percents[200-i]=i;

      /*
       * Set the frequency of Timer1 to 10Mhz with a reload value
       * of 50000. It will take 10e6/50000 = 200 ticks to get there
       * so the attached DMA channel will get 200 update events
       * per second.
       */

      timer.setTimeBaseByFrequency(10000000,49999);

      /*
       * Initialise channel 1's comparator for use as a PWM output
       */

      timer.initCompareForPwmOutput();

      /*
       * Start the timer
       */

      timer.enablePeripheral();

      /*
       * Attach the DMA channel to the timer and start it. The DMA
       * channel will automatically load each new duty cycle into
       * the CCR1 register on every update event. When it runs out
       * it will restart because it's in circular mode. The 'percents'
       * buffer does not need to remain in scope while the DMA operation
       * is running
       */

      dma.beginFadeByTimer(timer,percents,sizeof(percents));

      /*
       * It's all running automatically now - go and do something
       * cool with the CPU!
       */

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  TimerDmaPwmTest test;
  test.run();

  // not reached
  return 0;
}
