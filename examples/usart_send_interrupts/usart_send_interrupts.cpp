/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: asynchronous sending data using interrupts
 *
 * This example will send the string "Hello World" using
 * USART1. Connect an RS232 cable from USART1 to your PC
 * and run a terminal program (there are many free
 * terminal programs) to see the data. The default
 * (unremapped) USART1 pins are used.
 *
 * We use interrupts to send the data. The hardware
 * raises a TXE interrupt when it's ready to send a
 * character and we do just that. When we get to the end
 * of the string we disable the TXE interrupt so it
 * doesn't fire again.
 *
 * Note that if you are using the STM32F4DISCOVERY
 * board then you cannot use Usart1 since the pins clash
 * with onboard peripherals. I have tested this code on
 * that board using Usart2.
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

class UsartSendInterruptsTest {

  protected:

    /*
     * The string we are going to send
     */

    const char *_dataToSend;

    /*
     * The USART1 peripheral configured with the interrupt feature
     */

    typedef Usart1InterruptFeature MyUsartInterrupt;
    Usart1<MyUsartInterrupt> _usart;

  public:

    /*
     * Use the constructor base initialiser to set up the USART at 57600
     */

    UsartSendInterruptsTest()
      : _usart(57600) {
    }

    /*
     * Run the test
     */

    void run()  {

      // we're using interrupts, set up the NVIC

      Nvic::initialise();

      // set the initial string pointer

      _dataToSend="Hello World";

      // register ourselves as an observer of the USART interrupts

      _usart.UsartInterruptEventSender.insertSubscriber(
          UsartInterruptEventSourceSlot::bind(this,&UsartSendInterruptsTest::onInterrupt)
        );

      // enable interrupts. this will cause an immediate ready-to-send interrupt

      _usart.enableInterrupts(MyUsartInterrupt::TRANSMIT);

      // finished

      for(;;);
    }


    /*
     * Interrupt callback function. This is called when the TXE interrupt that we've
     * enabled is fired.
     */

    void onInterrupt(UsartEventType uet) {

      if(uet==UsartEventType::EVENT_READY_TO_TRANSMIT && *_dataToSend) {

        // send the next character and increment the pointer

        _usart.send(*_dataToSend++);

        // if we are now at the end of the string then disable further interrupts
        // because we are done

        if(*_dataToSend=='\0')
          _usart.disableInterrupts(MyUsartInterrupt::TRANSMIT);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  UsartSendInterruptsTest test;
  test.run();

  // not reached
  return 0;
}
