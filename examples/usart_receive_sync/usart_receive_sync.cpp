/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: synchronous receiving data
 *
 * This example will receive 5 characters from the PC
 * over USART1 and then immediately echo them back.
 * Connect an RS232 cable from USART1 to your PC and
 * run a terminal program (there are many free terminal
 * programs) and type 5 characters into it. Those
 * 5 characters will be echo'd back to the terminal.
 *
 * Note that if you are using the STM32F4DISCOVERY
 * board then you cannot use Usart1 since the pins
 * clash with onboard peripherals. I have tested
 * this code on that board using Usart2 and Uart4.
 *
 * The protocol is 57600/8/N/1
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F100RBT6
 *   STM32F103VET6
 *   STM32F407VGT6
 */

class UsartReceiveSyncTest {

  public:

    void run() {

      /*
       * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
       * if your application demands that you use the alternate pins for USART1
       */

      Usart1<> usart(57600);

      /*
       * We'll use streams to send and receive the data.
       */

      UsartPollingOutputStream outputStream(usart);
      UsartPollingInputStream inputStream(usart);

      /*
       * Go into a loop reading 5 characters at a time and then writing them
       * right back again.
       */

      for(;;) {

        uint8_t buffer[5];
        uint32_t actuallyRead;

        if(!inputStream.read(buffer,sizeof(buffer),actuallyRead) || actuallyRead!=sizeof(buffer)) {
          // read error: handle it here
        }

        if(!outputStream.write(buffer,sizeof(buffer))) {
          // write error: handle it here
        }

      }
    }
};


/*
 * Main entry point
 */

int main() {

  UsartReceiveSyncTest test;
  test.run();

  // not reached
  return 0;
}
