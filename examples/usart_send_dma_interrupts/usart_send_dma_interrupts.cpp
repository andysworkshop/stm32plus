/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


/**
 * USART test: asynchronous sending multiple sequences of data using DMA to transfer it
 * and using DMA interrupts to move through the sequence of data items to transfer
 *
 * This example will send the first few words from the famous Lorem Ipsum quotation using
 * USART1. Connect an RS232 cable from USART1 to your PC and run a terminal program
 * (there are many free terminal programs) to see the data. The default (unremapped) USART1
 * pins are used.
 *
 * The DMA channel associated with USART1 Tx is set up and triggered. When the first transfer
 * is complete an interrupt fires and we use to initiate the next transfer. When all transfers
 * are complete we lock up.
 *
 * Note that if you are using the STM32F4DISCOVERY board then you cannot use USART1 since the
 * pins clash with onboard peripherals. I have tested this code on that board using USART2.
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

static const char *_loremIpsum[]={
  "Lorem ","ipsum ","dolor ","sit amet, ","consectetur ","adipisicing ","elit, ","sed ","do ",
  "eiusmod ","tempor ","incididunt ","ut ","labore ","et ","dolore ","magna ","aliqua."
};


class UsartSendInterruptsTest {

  protected:

    /*
     * The index of the string that we are sending
     */

    uint8_t _currentlySending;


    /*
     * The DMA peripheral appropriate for this USART, configured for transmitting
     */

    typedef Usart1TxDmaChannel<                       // use the correct channel
      UsartDmaWriterFeature<Usart1PeripheralTraits>,   // we will be writing to it
      Usart1TxDmaChannelInterruptFeature              // we want its interrupts to fire
    > MyUsartTxDmaChannel;

    MyUsartTxDmaChannel *_dma;

  public:

    /*
     * Run the test
     */

    void run()  {

      /*
       * We're using interrupts, set up the NVIC
       */

      Nvic::initialise();

      /*
       * Declare a USART1 object. Note that an alternative Usart1_Remap object is available
       * if your application demands that you use the alternate pins for USART1. Include the
       * feature class for writing to the USART over DMA.
       */

      Usart1<> usart(57600);

      // declare the DMA channel for the USART. Must come after the USART peripheral is set up.

      _dma=new MyUsartTxDmaChannel();

      // enable DMA interrupts and set ourselves up to observe them

      _dma->enableInterrupts(Usart1TxDmaChannelInterruptFeature::COMPLETE);
      _dma->DmaInterruptEventSender.insertSubscriber(
          DmaInterruptEventSourceSlot::bind(this,&UsartSendInterruptsTest::onInterrupt)
        );

      // start the first transfer

      _currentlySending=0;

      _dma->beginWrite(_loremIpsum[0],strlen(_loremIpsum[0]));

      // it's in the background from now on

      for(;;);
    }


    /*
     * DMA Interrupt callback function. This is called when the completion
     * interrupt that we've enabled is fired.
     */

    void onInterrupt(DmaEventType det) {

      if(det==DmaEventType::EVENT_COMPLETE) {

        // update to the next word

        _currentlySending++;

        // only start another if there is more to go

        if(_currentlySending<sizeof(_loremIpsum)/sizeof(_loremIpsum[0])) {

          // clear the completion flag and send the next word

          _dma->clearCompleteFlag();
          _dma->beginWrite(_loremIpsum[_currentlySending],strlen(_loremIpsum[_currentlySending]));
        }
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
