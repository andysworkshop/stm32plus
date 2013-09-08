/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * This demo illustrates sending and receiving using the
 * SPI peripherals. A block of test data is sent from
 * SPI1 to SPI2 and, if successfully received, then a LED
 * on PF6 is flashed for 1 second. The test repeats
 * continuously.
 *
 * The SPI interrupts are used to send and receive the
 * data. Note the code that adjusts the relative
 * interrupt priorities.
 *
 * If you intend to run this example on the
 * STM23F4DISCOVERY board then replace PF6 with PD13 to
 * use the onboard LED.
 *
 * For this demo I'm going to need you to do a little
 * wiring to hook up SPI1 to SPI2 so that we can
 * exchange data over the MOSI pin. Here's the
 * connections that you need to make.
 *
 * 1MOSI/2MOSI: PA7  => PB15
 * NSS:         PA4 <=> PB12
 * SCLK:        PA5 <=> PB13
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */


static const uint8_t *dataToSend=(const uint8_t *)"Hello World";


class SpiSendInterruptsTest {

  protected:

    enum { LED_PIN = 6 };

    /*
     * These instance variables will be used by the interrupt to
     * co-ordinate the send and receive process.
     */

    volatile bool _completed;
    uint8_t _receiveBuffer[12];
    uint8_t _receiveIndex;
    uint8_t _sendIndex;


    /*
     * Declare the SPI sender/receiver.
     */

    typedef Spi1<Spi1InterruptFeature> MySender;
    typedef Spi2<Spi2InterruptFeature> MyReceiver;

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
       * Create the sender and receiver objects with default parameters
       */

      MySender::Parameters senderParams;
      MyReceiver::Parameters receiverParams;

      senderParams.spi_mode=SPI_Mode_Master;
      receiverParams.spi_mode=SPI_Mode_Slave;

      _sender=new MySender(senderParams);
      _receiver=new MyReceiver(receiverParams);

      /*
       * We need to adjust the priority of the interrupts so that the receiver is higher (lower number)
       * than the sender. If this is not done then the TXE interrupt (sender) will always pre-empt the
       * receiver with the undesirable result that all the bytes will be transmitted but only the first
       * will be received because the receiver IRQ will fire on the first byte transmission and then get
       * queued until transmission of all bytes finish by which time all but the queued receiver interrupt
       * have been missed.
       */

      _sender->setNvicPriorities(1);
      _receiver->setNvicPriorities(0);

      /*
       * register ourselves as an observer of the SPI interrupts
       */

      _sender->SpiInterruptEventSender.insertSubscriber(
          SpiInterruptEventSourceSlot::bind(this,&SpiSendInterruptsTest::onInterrupt)
        );

      _receiver->SpiInterruptEventSender.insertSubscriber(
          SpiInterruptEventSourceSlot::bind(this,&SpiSendInterruptsTest::onInterrupt)
        );

      for(;;) {

        /*
         * Reset the instance variables
         */

        _sendIndex=0;
        _receiveIndex=0;
        _completed=false;

        memset(_receiveBuffer,0,sizeof(_receiveBuffer));

        /*
         * NSS (slave select) is active LOW. ST made such a mess of the hardware implementation of NSS
         * that we always control it through software. Here it's pulled LOW ready for transmission.
         */

        _sender->setNss(false);

        /*
         * Enable them now. TXE will be raised immediately
         */

        _receiver->enableInterrupts(SPI_I2S_IT_RXNE);
        _sender->enableInterrupts(SPI_I2S_IT_TXE);

        /*
         * Wait for the interrupts to signal completion
         */

        while(!_completed);

        /*
         * De-select NSS
         */

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

      if(set==SpiEventType::EVENT_READY_TO_TRANSMIT) {

        // send a byte and increment send position

        _sender->send(&dataToSend[_sendIndex],1);
        _sendIndex++;

        // if all is sent, disable further send interrupts

        if(_sendIndex==sizeof(_receiveBuffer))
          _sender->disableInterrupts(SPI_I2S_IT_TXE);
      }
      else if(set==SpiEventType::EVENT_RECEIVE) {

        // receive a byte

        _receiver->receive(_receiveBuffer[_receiveIndex]);
        _receiveIndex++;

        // if all is received, disable interrupts and signal finished

        if(_receiveIndex==sizeof(_receiveBuffer)) {
          _receiver->disableInterrupts(SPI_I2S_IT_RXNE);
          _completed=true;
        }
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
