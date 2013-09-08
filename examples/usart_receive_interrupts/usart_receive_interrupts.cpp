/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: asynchronous sending and receiving data
 * using interrupts
 *
 * This example will receive 5 characters on USART1 and
 * then immediately echo them back. Connect an RS232
 * cable from USART1 to your PC and run a terminal
 * program (there are many free terminal programs) to
 * see the data. The default (unremapped) USART1 pins
 * are used.
 *
 * We use interrupts to send and receive the data. The
 * hardware raises a TXE interrupt when it's ready to
 * send a character and an RXNE interrupt when data is
 * ready to receive.
 *
 * Note that if you are using the STM32F4DISCOVERY board
 * then you cannot use Usart1 since the pins clash with
 * onboard peripherals. I have tested this code on that
 * board using Usart2.
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

class UsartReceiveInterruptsTest {

  protected:

    /*
     * We'll use this as our simple I/O buffer
     */

    uint8_t _buffer[5];
    uint8_t _index;

    /*
     * The USART1 peripheral configured with the interrupt feature
     */

    typedef Usart1InterruptFeature MyUsartInterrupt;
    Usart1<MyUsartInterrupt> _usart;

  public:

    /*
     * Use the constructor base initialiser to set up the USART at 57600
     */

    UsartReceiveInterruptsTest()
      :_usart(57600) {
    }

    /*
     * Run the test
     */

    void run()  {

      /*
       * We're using interrupts, set up the NVIC
       */

      Nvic::initialise();

      // register ourselves as an observer of the USART interrupts

      _usart.UsartInterruptEventSender.insertSubscriber(
          UsartInterruptEventSourceSlot::bind(this,&UsartReceiveInterruptsTest::onInterrupt)
        );

      // enable the receive interrupt. this will start the whole chain of events

      _index=0;
      _usart.enableInterrupts(MyUsartInterrupt::RECEIVE);

      // it's all going on in the background now. wish us luck :)

      for(;;);
    }


    /*
     * Interrupt callback function. This is called when the TXE interrupt that we've
     * enabled is fired.
     */

    void onInterrupt(UsartEventType uet) {

      if(uet==UsartEventType::EVENT_RECEIVE) {

        // receive the next character

        _buffer[_index++]=_usart.receive();

        // if we've got the 5 characters then disable receiving interrupts
        // and enable sending

        if(_index==5) {
          _index=0;
          _usart.disableInterrupts(MyUsartInterrupt::RECEIVE);
          _usart.enableInterrupts(MyUsartInterrupt::TRANSMIT);
        }
      }
      else if(uet==UsartEventType::EVENT_READY_TO_TRANSMIT) {

        // send the next character

        _usart.send(_buffer[_index++]);

        // if we've sent back all 5 then disable sending interrupts and go back
        // to receiving again

        if(_index==5) {
          _index=0;
          _usart.disableInterrupts(MyUsartInterrupt::TRANSMIT);
          _usart.enableInterrupts(MyUsartInterrupt::RECEIVE);
        }
      }
    }
};


/*
 * Main entry point
 */

int main() {

  UsartReceiveInterruptsTest test;
  test.run();

  // not reached
  return 0;
}
