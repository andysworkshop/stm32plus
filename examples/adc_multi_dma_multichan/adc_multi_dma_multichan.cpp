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
 * This example shows how to use two ADC peripherals simultaneously. ADC1 and ADC2 are used
 * to convert channels 0,1 (ADC1) and 2,3 (ADC2) simultaneously. That is, channels 0 and 2
 * are converted at the same time followed by channels 1 and 3. The order of the values in
 * the 4-value output buffer will be channel [0,2,1,3].
 *
 * USART3 remap1 is configured with protocol settings of 57600/8/N/1. The ADC channels are read
 * from PA[0], PA[1], PA[2], PA[3]. You will need to connect these GPIO inputs to valid levels
 * between GND and VREF to see conversion values.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F103ZET6
 *   STM32F407VGT6
 */


class AdcMultiDmaMultiChan {

  private:
    volatile bool _ready;

  public:

    void run() {

      uint16_t dmaTransfers;

      _ready=false;

      /*
       * We're converting 4 channels in circular buffer mode so we need exactly 4 16-bit words
       * for that circular buffer
       */

      volatile uint16_t readBuffer[4];

      /*
       * Unfortunately the ADC is quite different across the MCU series so we have to
       * be MCU-specific when declaring this instance
       */

#if defined(STM32PLUS_F1)

      /*
       * Declare the ADC DMA channel. The default is circular mode for the MultiAdcDmaMode1Feature
       * which means that it wil automatically refill our buffer on each conversion because
       * one conversion exactly matches the size of the memory buffer that we will give
       * to the DMA peripheral.
       */

      Adc1DmaChannel<AdcMultiDmaFeature<Adc1PeripheralTraits>,Adc1DmaChannelInterruptFeature> dma;

      Adc1<
        AdcClockPrescalerFeature<6>,                    // PCLK2/6
        Adc1Cycle55RegularChannelFeature<0,1>,          // using channels 0,1 on ADC1 with 55.5-cycle latency
        AdcScanModeFeature,                             // scan mode
        AdcContinuousModeFeature,
        AdcDualRegularSimultaneousFeature<              // regular simultaneous multi mode
          Adc2<                                         // the second ADC
            Adc2Cycle55RegularChannelFeature<2,3>,      // using channels 2,3 on ADC2 with 55.5-cycle latency
            AdcScanModeFeature,                         // scan mode
            AdcContinuousModeFeature
          >
        >
      > adc;

      // word transfers

      dmaTransfers=2;

#elif defined(STM32PLUS_F4)

      /*
       * Declare the ADC DMA channel. The default is circular mode for the MultiAdcDmaMode1Feature
       * which means that it wil automatically refill our buffer on each conversion because
       * one conversion exactly matches the size of the memory buffer that we will give
       * to the DMA peripheral.
       */

      Adc1DmaChannel<AdcMultiDmaMode1Feature<Adc1PeripheralTraits>,Adc1DmaChannelInterruptFeature> dma;

      /*
       * Declare the ADC1 peripheral with an APB2 clock prescaler of 2, a resolution of 12 bits.
       * We will use 144-cycle conversions on ADC channels 0,1. Scan mode is used with the default
       * template parameter that causes EOC to be raised at the end of a complete conversion group.
       * ADC1 is the master so it must be the one that declares multi-mode. We're using
       * 'regular-simultaneous' multi-mode with DMA mode 1 and a 5-cycle min delay between 2 conversions.
       * The slave ADC (ADC2) is given as a type parameter to the dual ADC feature. This feature will
       * take care of instantiating it as a slave at the appropriate time.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,                    // prescaler of 2
        AdcResolutionFeature<12>,                       // 12 bit resolution
        Adc1Cycle144RegularChannelFeature<0,1>,         // using channels 0,1 on ADC1 with 144-cycle latency
        AdcScanModeFeature<>,                           // scan mode with EOC after each group
        AdcDualRegularSimultaneousDmaMode1Feature<      // regular simultaneous multi mode
          Adc2<                                         // the second ADC
            AdcClockPrescalerFeature<2>,                // prescaler of 2
            AdcResolutionFeature<12>,                   // 12 bit resolution
            Adc2Cycle144RegularChannelFeature<2,3>,     // using channels 2,3 on ADC2 with 144-cycle latency
            AdcScanModeFeature<>                        // scan mode with EOC after each group
          >,
          5                                             // 5 cycle min delay
        >
      > adc;

      // half-word transfers

      dmaTransfers=4;

#endif

      /*
       * Subscribe to the DMA complete interrupt
       */

      dma.DmaInterruptEventSender.insertSubscriber(
          DmaInterruptEventSourceSlot::bind(this,&AdcMultiDmaMultiChan::onComplete)
      );

      /*
       * Enable the DMA interrupt
       */

      dma.enableInterrupts(Adc1DmaChannelInterruptFeature::COMPLETE);

      /*
       * Declare an instance of USART that we'll use to write out the conversion results.
       */

      Usart3_Remap1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /**
       * start the DMA (i.e. make it ready to receive requests from the ADC peripheral)
       */

      dma.beginRead(readBuffer,dmaTransfers);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        /*
         * For this test code we set the 4 16-bit words to a known pattern so we can tell if they
         * get properly overwritten by the DMA during conversion
         */

        readBuffer[0]=readBuffer[1]=readBuffer[2]=readBuffer[3]=0xAAAA;

        /*
         * Start a conversion and wait until the interrupt handler tells us
         * that it's finished.
         */

        adc.startRegularConversion();

        while(!_ready);
        _ready=false;

        /*
         * write the values to the USART
         */

        outputStream << "Converted values are "
                     << StringUtil::Ascii(readBuffer[0]) << ", "
                     << StringUtil::Ascii(readBuffer[1]) << ", "
                     << StringUtil::Ascii(readBuffer[2]) << ", "
                     << StringUtil::Ascii(readBuffer[3]) << "\r\n";

        // wait for a second before converting the next set of values

        MillisecondTimer::delay(1000);
      }
    }


    /**
     * Interrupt handler for the DMA complete event. Set the ready flag
     * when it's received.
     */

    void onComplete(DmaEventType det) {
      if(det==DmaEventType::EVENT_COMPLETE)
        _ready=true;
    }
};

int main() {

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  MillisecondTimer::initialise();

  AdcMultiDmaMultiChan adc;
  adc.run();

  // not reached
  return 0;
}
