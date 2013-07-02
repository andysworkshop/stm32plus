/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"
#include "config/usart.h"
#include "config/dma.h"


using namespace stm32plus;


/**
 * This example shows how to use a timer linked to a DMA
 * channel to transmit data automatically
 * to the USART peripheral.
 *
 * Timer1's update event is configured to fire once per
 * second. That update event is linked to a DMA channel
 * that targets the USART1 peripheral's TX register. For
 * our demo purposes we transmit a sample text string
 * continuously at a rate of 1 character per second.
 *
 * USART1 is used in default (unremapped) configuration:
 * 4800-8-N-1
 *
 * Note that if you are using the STM32F4DISCOVERY board
 * then you cannot use Usart1 since the pins clash with
 * onboard peripherals. I have tested this code on that
 * board using Usart2.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class TimerDmaUsartTest {

  public:

    void run() {

      const char *dataToSend="Hello World";

      /*
       * We're using USART1 (APB2), unremapped. Configure it to run at 4800 baud, 8 bits
       * no flow control and 1 stop bit. (4800-8-N-1).
       */

      Usart1<> usart1(4800);

      /*
       * Configure Timer1 (APB2) with the internal clock source. No further features
       * are required.
       */

      Timer1<
        Timer1InternalClockFeature        // the timer bus is APB2
      > timer;

      /*
       * Create an instance of the DMA channel that is connected to
       * Timer1's update event. Add a feature that connects the timer's update
       * event to the USART1 TX peripheral register.
       *
       * This will be a circular DMA configuration, i.e. it will automatically
       * run itself over and over again until we stop it.
       */

      Timer1UpdateDmaChannel<
        TimerUpdateDmaFeature<Usart1TxDmaPeripheralInfo,DMA_Priority_High,DMA_Mode_Circular>
      > dma;

      /*
       * Set the frequency of Timer1 to 50KHz with a reload value
       * of 50000. It will take 1 second to get from zero to 50000
       * so the attached DMA channel will get 1 update event per second.
       */

      timer.setTimeBaseByFrequency(50000,49999);

      /*
       * Start the timer
       */

      timer.enablePeripheral();

      /*
       * Attach the DMA channel to the timer and start it. The DMA
       * channel will automatically load the next character to transmit
       * into the USART register.
       */

      dma.beginWriteByTimer(timer,dataToSend,strlen(dataToSend));

      /*
       * It's all running in hardware now, the CPU is ours to do what we want with. I've run
       * out of bright ideas so I'll just do nothing.
       */

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  TimerDmaUsartTest test;
  test.run();

  // not reached
  return 0;
}
