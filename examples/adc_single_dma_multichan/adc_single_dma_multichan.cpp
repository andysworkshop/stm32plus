/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/adc.h"
#include "config/usart.h"
#include "config/string.h"


using namespace stm32plus;


/**
 * This example adds DMA and multi-channel conversion to the mix. We'll use ADC1 to convert
 * three channels automatically and in sequence and we'll write out the results to the
 * USART for you to see.
 *
 * The ADC is configured in 'scan mode' which means that it will convert all the configured
 * channels and, because we are not using continuous mode, it will stop at the end of the
 * group. The DMA channel for ADC1 is used to move the converted channel data out to SRAM.
 * We configure the end-of-conversion interrupt (EOC) to fire when the complete group has
 * finished converting. The converted data is written to the USART, we pause for a second
 * and then do it all again, ad infinitum.
 *
 * USART1 is configured with protocol settings of 57600/8/N/1. The ADC channels are read
 * from PA[0], PA[1], PA[2]. You will need to connect these GPIO inputs to valid levels
 * between GND and VREF to see conversion values.
 *
 * Compatible MCU:
 *   STM32F0
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 *   STM32F107VCT6
 */

class AdcSingleDmaMultiChan {

  private:
    volatile bool _ready;

  public:

    void run() {

      _ready=false;

      /*
       * We're converting 3 channels in circular buffer mode so we need exactly 3 16-bit words
       * for that circular buffer
       */

      uint16_t readBuffer[3];

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
       * 12 bits. We will use 3-cycle conversions on ADC channel 0. We will use interrupts
       * to tell us when the conversion is complete.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,              // prescaler of 2
        AdcResolutionFeature<12>,                 // 12 bit resolution
        Adc1Cycle3RegularChannelFeature<0,1,2>,   // using channels 0,1 and 2 on ADC1 with 3-cycle latency
        AdcScanModeFeature<AdcScanModeEndType::END_AFTER_WHOLE_GROUP>,    // scan mode with end interrupt after each group
        Adc1InterruptFeature                      // enable interrupt handling on this ADC
      > adc;

      /*
       * Subscribe to the interrupts raised by the ADC
       */

      adc.AdcInterruptEventSender.insertSubscriber(
          AdcInterruptEventSourceSlot::bind(this,&AdcSingleDmaMultiChan::onInterrupt)
        );

      /*
       * Declare the ADC1 DMA channel. The default is circular mode for the AdcDmaFeature
       * which means that it wil automatically refill our buffer on each conversion because
       * one conversion exactly matches the size of the memory buffer that we will give
       * to the DMA peripheral.
       */

      Adc1DmaChannel<AdcDmaFeature<Adc1PeripheralTraits>> dma;

      /*
       * Declare an instance of USART that we'll use to write out the conversion results.
       */

      Usart1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /**
       * Enable the ADC interrupt and start the DMA
       */

      adc.enableInterrupts(ADC_IT_EOC);
      dma.beginRead(readBuffer,sizeof(readBuffer));

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        /*
         * For this test code we set the 3 words to a known pattern so we can tell if they
         * get properly overwritten by the DMA during conversion
         */

        readBuffer[0]=readBuffer[1]=readBuffer[2]=0xAAAA;

        /*
         * Start a conversion and wait until the interrupt handler tells us
         * that it's finished.
         */

        adc.startRegularConversion();

        while(!_ready);
        _ready=false;

        /*
         * write the 3 values to the USART
         */

        outputStream << "Converted values are "
                     << StringUtil::Ascii(readBuffer[0]) << ", "
                     << StringUtil::Ascii(readBuffer[1]) << ", "
                     << StringUtil::Ascii(readBuffer[2]) << "\r\n";

        // wait for a second before converting the next set of values

        MillisecondTimer::delay(1000);
      }
    }


    /**
     * Interrupt handler for the EOC event. If the event and the ADC number
     * match then we set the ready flag. This will release the main code
     * that's waiting on it.
     */

    void onInterrupt(AdcEventType eventType,uint8_t adcNumber) {

      if(eventType==AdcEventType::EVENT_REGULAR_END_OF_CONVERSION && adcNumber==1) {
        _ready=true;
      }
    }
};


/*
 * Main entry point
 */

int main() {

  MillisecondTimer::initialise();

  AdcSingleDmaMultiChan adc;
  adc.run();

  // not reached
  return 0;
}
