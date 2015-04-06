/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * This demo illustrates sending and receiving using the SPI peripherals. A block of test
 * data is sent from SPI2 to SPI1 and, if successfully received, then a LED on PF6 is
 * flashed for 1 second. The test repeats continuously.
 *
 * SPI2 is used to send the data and it's received by the SPI1 interrupt.
 *
 * If you intend to run this example on the F4 DISCOVERY board then replace PF6 with PD13
 * to use the onboard LED.
 *
 * If you intend to run this example on the VL or F0 DISCOVERY board then replace PF6 with PC8 to
 * use the onboard LED.
 *
 * For this demo I'm going to need you to do a little wiring to hook up SPI1 to SPI2 so that
 * we can exchange data over the MOSI pin. Here are the connections that you need to make.
 *
 * 1MOSI/2MOSI: PA7  => PB15
 * NSS:         PA4 <=> PB12
 * SCLK:        PA5 <=> PB13
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
 *   STM32F100RBT6
 */


static const uint8_t *dataToSend=(const uint8_t *)"Hello World";


class SpiSendInterruptsTest {

  protected:

    enum { LED_PIN = 6 };

    /*
     * These instance variables will be used by the interrupt to
     * co-ordinate the send and receive process.
     */

    uint8_t _receiveBuffer[12];
    volatile uint8_t _receiveIndex;
    uint8_t _sendIndex;

    /*
     * Declare the SPI sender/receiver.
     */

    typedef Spi2<> MySender;
    typedef Spi1<Spi1InterruptFeature> MyReceiver;

    MySender *_sender;
    MyReceiver *_receiver;

  public:

    void run() {

      // initialise the LED on PF6. It's active LOW so we set it HIGH to turn it off

      GpioF<DefaultDigitalOutputFeature<LED_PIN> > pf;
      pf[LED_PIN].set();

      /*
       * We're using interrupts, set up the NVIC
       */

      Nvic::initialise();

      /**
       * Create the sender and receiver objects as master and slave
       */

      MySender::Parameters senderParams;
      MyReceiver::Parameters receiverParams;

      senderParams.spi_mode=SPI_Mode_Master;
      receiverParams.spi_mode=SPI_Mode_Slave;

      _sender=new MySender(senderParams);
      _receiver=new MyReceiver(receiverParams);

      /*
       * register ourselves as an observer of the SPI receive interrupt
       */

      _receiver->SpiInterruptEventSender.insertSubscriber(
          SpiInterruptEventSourceSlot::bind(this,&SpiSendInterruptsTest::onInterrupt)
        );

      for(;;) {

        uint8_t i;

        /*
         * Reset the instance variables
         */

        _sendIndex=0;
        _receiveIndex=0;

        memset(_receiveBuffer,0,sizeof(_receiveBuffer));

        /*
         * Enable the receive interrupt
         */

        _receiver->enableInterrupts(SPI_I2S_IT_RXNE);

        /*
         * NSS (slave select) is active LOW. ST made such a mess of the hardware implementation of NSS
         * that we always control it through software. Here it's pulled LOW ready for transmission.
         */

        _sender->setNss(false);

        /*
         * Send the block of data from SPI2 to SPI1. Note the lack of flow control. Don't built a
         * real system like this.
         */

        for(i=0;i<12;i++) {

          /*
           * Wait for the sender to signal it's ready and then send a byte
           */

          while(!_sender->readyToSend());
          _sender->send(&dataToSend[i],1);
        }

        /*
         * Wait for the receive interrupt handler to signal that it's finished
         */

        while(_receiveIndex!=12);

        /*
         * Wait for idle and de-select NSS
         */

        _sender->waitForIdle();
        _sender->setNss(true);

        /*
         * Test the received buffer. If the data is incorrect then lock up
         */

        if(memcmp(_receiveBuffer,dataToSend,sizeof(_receiveBuffer))!=0)
          for(;;);

        /*
         * The data is correct, flash the LED on PF6 for one second
         */

        pf[LED_PIN].reset();
        MillisecondTimer::delay(1000);
        pf[LED_PIN].set();
        MillisecondTimer::delay(1000);
      }
    }


    /*
     * Interrupt callback function. This is called when the TXE and RXNE interrupts that we've
     * enabled are fired.
     */

    void onInterrupt(SpiEventType set) {

      if(set==SpiEventType::EVENT_RECEIVE) {

        // receive a byte

        _receiver->receive(_receiveBuffer[_receiveIndex]);
        _receiveIndex++;

        // if all is received, disable interrupts and signal finished

        if(_receiveIndex==sizeof(_receiveBuffer))
          _receiver->disableInterrupts(SPI_I2S_IT_RXNE);
      }
    }
};


/*
 * Main entry point
 */

int main() {

  MillisecondTimer::initialise();

  SpiSendInterruptsTest test;
  test.run();

  // not reached
  return 0;
}
