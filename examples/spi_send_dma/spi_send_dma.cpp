/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"
#include "config/dma.h"
#include "config/timing.h"


using namespace stm32plus;


/**
 * This demo illustrates sending and receiving using the SPI peripherals. A block
 * of test data is sent from SPI1 to SPI2 and, if successfully received, then a LED
 * on PF6 is flashed for 1 second. The test repeats continuously.
 *
 * The DMA channels assigned to SPI1 and SPI2 are used to perform the transfer.
 *
 * If you intend to run this example on the F4 DISCOVERY board then replace PF6
 * with PD13 to use the onboard LED.
 *
 * If you intend to run this example on the VL or F0 DISCOVERY board then replace
 * PF6 with PC8 to use the onboard LED.
 *
 * For this demo I'm going to need you to do a little wiring to hook up SPI1 to SPI2
 * so that we can exchange data over the MOSI pin. Here are the connections that
 * you need to make.
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
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 */

class SpiSendDmaTest {

  protected:

    enum { LED_PIN = 6 };

  public:

    void run() {

      const uint8_t *dataToSend=(const uint8_t *)"Hello World";
      uint8_t receiveBuffer[12];

      // initialise the LED on PF6. It's active LOW so we set it HIGH to turn it off

      GpioF<DefaultDigitalOutputFeature<LED_PIN> > pf;
      pf[LED_PIN].set();

      /*
       * Declare our SPI objects with no extra features beyond the ability to send and
       * receive data bytes. SPI1 is going to be the master and SPI2 the slave. All the SPI
       * remap configurations are available with names such as Spi1_Remap if you need to use the
       * remapped pins.
       */

      typedef Spi1<> MySender;
      typedef Spi2<> MyReceiver;

      MySender::Parameters senderParams;
      MyReceiver::Parameters receiverParams;

      senderParams.spi_mode=SPI_Mode_Master;
      receiverParams.spi_mode=SPI_Mode_Slave;

      MySender sender(senderParams);
      MyReceiver receiver(receiverParams);

      /*
       * Declare the transmit and receive DMA channels assigned to the two peripherals.
       * We'll give reception a higher priority than transmit since we are both the
       * sender and the receiver.
       *
       * On the F4 the DMA FIFO is enabled by default. If you want to disable the FIFO then
       * you can do that with an additional template parameter to SpiDmaWriterFeature and
       * SpiDmaReaderFeature.
       */

      Spi1TxDmaChannel<SpiDmaWriterFeature<Spi1PeripheralTraits,DMA_Priority_Medium> > dmaSender;
      Spi2RxDmaChannel<SpiDmaReaderFeature<Spi2PeripheralTraits,DMA_Priority_High> > dmaReceiver;

      for(;;) {

        /*
         * Clear out the receive buffer for this session
         */

        memset(receiveBuffer,0,12);

        /*
         * Start the DMA receiver ready for the data
         */

        dmaReceiver.beginRead(receiveBuffer,12);

        /*
         * NSS (slave select) is active LOW. ST made such a mess of the hardware implementation of NSS
         * that we always control it through software. Here it's pulled LOW ready for transmission.
         */

        sender.setNss(false);

        /*
         * Start the DMA sender and let those bytes flow. The receiver has already been started and is
         * stalled waiting for this
         */

        dmaSender.beginWrite(dataToSend,12);

        /*
         * Wait until the receiver gets all the bytes
         */

        if(!dmaReceiver.waitUntilComplete()) {
          // handle error properly

          for(;;);
        }

        /*
         * Wait until the sender has signalled that it's finished
         */

        if(!dmaSender.waitUntilComplete()) {
          // handle error properly

          for(;;);
        }

        /*
         * The session is complete, wait for the last transmission then deactivate NSS.
         */

        sender.waitForIdle();
        sender.setNss(true);

        /*
         * Test the received buffer. If the data is incorrect then lock up
         */

        if(memcmp(receiveBuffer,dataToSend,12)!=0)
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
};


/*
 * Main entry point
 */

int main() {

  MillisecondTimer::initialise();

  SpiSendDmaTest test;
  test.run();

  // not reached
  return 0;
}
