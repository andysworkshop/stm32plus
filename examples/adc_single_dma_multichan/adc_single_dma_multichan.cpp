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

  public:

    void run() {

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
       * 12 bits. We will use 3-cycle conversions on ADC channel 0. We will use interrupts
       * to tell us when the conversion is complete.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,              // prescaler of 2
        AdcResolutionFeature<12>,                 // 12 bit resolution
        Adc1Cycle3RegularChannelFeature<0,1,2>,   // using channels 0,1 and 2 on ADC1 with 3-cycle latency
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
       * Enable the ADC interrupt
       */

      adc.enableInterrupts(ADC_IT_EOC);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        uint16_t value;

        // start a conversion and wait until the peripheral signals that it's started

        adc.startRegularConversion();
        while(!adc.hasRegularConversionStarted());

        // poll the EOC flag that tells us when the conversion has finished

        while(!adc.hasRegularConversionFinished());

        // get the result

        value=adc.getRegularConversionValue();

        // write the value to the USART

        outputStream << "Converted value is " << StringUtil::Ascii(value) << "\r\n";

        // wait for a second before converting the next value

        MillisecondTimer::delay(1000);
      }
    }


    void onInterrupt(AdcEventType eventType,uint8_t adcNumber) {
      if(eventType==AdcEventType::EVENT_REGULAR_END_OF_CONVERSION && adcNumber==1) {
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
