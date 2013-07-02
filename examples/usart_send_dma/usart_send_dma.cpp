/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: sending data using the DMA peripheral
 *
 * This example will send the string "Hello World"
 * using the DMA peripheral assigned to USART1. USART1
 * is using standard, unremapped pins.
 *
 * Note that if you are using the STM32F4DISCOVERY
 * board then you cannot use Usart1 since the pins
 * clash with onboard peripherals. I have tested this
 * code on that board using Usart2.
 *
 * The protocol is 57600/8/N/1
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class UsartSendDmaTest {

  public:

    void run() {

      const char *dataToSend="Hello World";

      /*
       * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
       * if your application demands that you use the alternate pins for USART1. Include the
       * feature class for writing to the USART over DMA.
       */

      Usart1<> usart(57600);

      /*
       * Attach the correct DMA channel to this USART. The DMA channel has a writer feature
       * attached to it, and that writer feature takes a template parameter of the USART
       * peripheral so it knows where it's going to be writing to.
       */

      Usart1TxDmaChannel<UsartDmaWriterFeature<Usart1PeripheralTraits> > dma;

      /*
       * Write out the test string and wait for completion
       */

      dma.beginWrite(dataToSend,strlen(dataToSend));
      dma.waitUntilComplete();

      for(;;);
    }
};


/*
 * Main entry point
 */

int main() {

  UsartSendDmaTest test;
  test.run();

  // not reached
  return 0;
}
