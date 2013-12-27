/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * Demo of the DMA peripheral used to copy a block of memory. We repeatedly copy
 * a small 256 byte buffer using the completion interrupt to signal the end of
 * each transfer. If all is well then a LED on PF6 will blink once per second.
 *
 * If this example is to be run on the STM32F4DISCOVERY board then change the LED
 * configuration from PF6 to PD13 and invert the set() / reset() logic because
 * that LED is active HIGH.
 *
 * If this example is to be run on the F1 VL or the F0 DISCOVERY board then change
 * the LED configuration from PF6 to PC8 and invert the set() / reset() logic because
 * that LED is active HIGH.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F051R8T6
 *   STM32F103ZET6
 *   STM32F100RBT6
 *   STM32F407VGT6
 */

class DmaCopyTest {

  protected:

    /**
     * The LED is on PF6
     */

    enum { LED_PIN = 8 };

    /*
     * The IRQ handler sets this to true when the DMA transfer is complete
     */

    volatile bool _completed;

  public:

    void run() {

      uint8_t buffer1[256],buffer2[256];
      int i;

      // initialise the LED pin

      GpioC<DefaultDigitalOutputFeature<LED_PIN> > pc;

      // lights off (this LED is active low, i.e. PF6 is a sink)

      pc[LED_PIN].reset();

      // declare a DMA channel with interrupts and memory copy features
      // F4 users note that only DMA2 can do memory-to-memory transfers.

#if defined(STM32PLUS_F1) || defined(STM32PLUS_F0)

      Dma1Channel1<
        Dma1Channel1InterruptFeature,   // interrupts on DMA1, channel 1
        DmaMemoryCopyFeature<>          // memory copy with default transfer size (bytes)
      > dma;

      // enable the completion interrupt for DMA1, channel 1.

      dma.enableInterrupts(Dma1Channel1InterruptFeature::COMPLETE);

#elif defined(STM32PLUS_F4)

      Dma2Channel1Stream0<
        Dma2Stream0InterruptFeature,    // interrupts on DMA2, stream 0
        DmaMemoryCopyFeature<>          // memory copy with default transfer size (bytes)
      > dma;

      // enable the completion interrupt for DMA2, stream 0.

      dma.enableInterrupts(Dma2Stream0InterruptFeature::COMPLETE);

#endif

      // set ourselves up to observe the completion of the DMA transfer

      dma.DmaInterruptEventSender.insertSubscriber(
          DmaInterruptEventSourceSlot::bind(this,&DmaCopyTest::onComplete)
        );

      for(;;) {

        // reset the completion flag

        _completed=false;

        // clear the target buffer, fill the source buffer with a pattern

        memset(buffer2,'\0',sizeof(buffer2));
        for(i=0;i<256;i++)
          buffer1[i]=i;

        // start the transfer of 256 bytes from buffer1 to buffer2. this executes asynchronously.

        dma.beginCopyMemory(buffer2,buffer1,sizeof(buffer1),DMA_Priority_Medium);

        // wait for transfer complete via the IRQ

        while(!_completed);

        // verify the result

        for(i=0;i<256;i++)
          if(buffer2[i]!=i)
            for(;;);          // lock up on error

        // flash the LED for a second

        pc[LED_PIN].set();
        MillisecondTimer::delay(1000);
        pc[LED_PIN].reset();
        MillisecondTimer::delay(1000);
      }
    }


    /*
     * Observer callback is fired when the DMA transfer is complete
     */

    void onComplete(DmaEventType det) {
      if(det==DmaEventType::EVENT_COMPLETE)
        _completed=true;
    }
};


/*
 * Main entry point
 */

int main() {

  // set up SysTick at 1ms resolution
  MillisecondTimer::initialise();

  DmaCopyTest test;
  test.run();

  // not reached
  return 0;
}
