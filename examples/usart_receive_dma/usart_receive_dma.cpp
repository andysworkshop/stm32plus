/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: synchronous receiving data
 *
 * This example will receive 5 characters from the PC over USART1 using the DMA channel and then
 * immediately echo them back, also using the DMA channel.
 *
 * Connect an RS232 cable from USART1 to your PC and run a terminal program (there are many free
 * terminal programs) and type 5 characters into it. Those 5 characters will be echo'd back
 * to the terminal.
 *
 * Note that if you are using the STM32F4DISCOVERY board then you cannot use USART1 since the pins
 * clash with onboard peripherals. I have tested this code on that board using USART2.
 *
 * The protocol is 57600/8/N/1
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
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class UsartReceiveDmaTest {

  public:

    void run() {

      /*
       * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
       * if your application demands that you use the alternate pins for USART1
       */

      Usart1<> usart(57600);

      Usart1TxDmaChannel<UsartDmaWriterFeature<Usart1PeripheralTraits> > dmaWriter;
      Usart1RxDmaChannel<UsartDmaReaderFeature<Usart1PeripheralTraits> > dmaReader;

      /*
       * Go into a loop reading 5 characters at a time and then writing them
       * right back again.
       */

      for(;;) {

        uint8_t buffer[5];

        dmaReader.beginRead(buffer,sizeof(buffer));
        dmaReader.waitUntilComplete();

        dmaWriter.beginWrite(buffer,sizeof(buffer));
        dmaWriter.waitUntilComplete();
      }
    }
};


/*
 * Main entry point
 */

int main() {

  UsartReceiveDmaTest test;
  test.run();

  // not reached
  return 0;
}
