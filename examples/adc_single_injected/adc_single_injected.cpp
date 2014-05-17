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
 * This example shows how "injected" channels can be combined with regular channels using
 * scan mode.
 *
 * We will use ADC1 to convert channel 1 (regular) and channel 2 (injected) in
 * scan mode using the ADC1 interrupt to tell us when all the data is ready.
 *
 * When the values have been converted the ADC1 peripheral will raise an interrupt and we will
 * write out the values to a USART. The USART is configured as 56000/8/N/1.
 *
 * If you want to see some real values then you'll need to wire PA0 (ADC123_IN0) and
 * PA1 (ADC123_IN1) to valid levels, or GND/VCC if you want to see values close to 0 and 4095.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F107VCT6
 *   STM32F407VGT6
 */


class AdcSingleInjected {

  private:
    volatile bool _ready;
    uint16_t _values[2];

    /*
     * Unfortunately the ADC is quite different across the MCU series so we have to
     * be MCU-specific when declaring this instance
     */

#if defined(STM32PLUS_F1)

      /*
       * Declare the ADC peripheral with a PCLK2 clock prescaler of 6. The ADC clock cannot exceed 14MHz so
       * if PCLK2 is 72MHz then we're operating it at 12MHz here.
       */

      Adc1<
        AdcClockPrescalerFeature<6>,              // PCLK2/6
        Adc1Cycle71RegularChannelFeature<0>,
        Adc1Cycle71InjectedChannelFeature<1>,
        Adc1InterruptFeature,
        AdcScanModeFeature
      > _adc;

#elif defined(STM32PLUS_F4)

    /*
     * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
     * 12 bits. We will use 144-cycle conversions on ADC channels 0,1 and a 480-cycle
     * conversion on ADC channel 2. Scan mode is used with the default template parameter
     * that causes EOC to be raised at the end of a complete conversion group.
     */

    Adc1<
      AdcClockPrescalerFeature<2>,                // prescaler of 2
      AdcResolutionFeature<12>,                   // 12 bit resolution
      Adc1Cycle144RegularChannelFeature<0>,       // using regular channel 0 on ADC1 with 144-cycle latency
      Adc1Cycle144InjectedChannelFeature<1>,      // using injected channel 1 on ADC1 with 144-cycle latency
      Adc1InterruptFeature,                       // using interrupts to signal end of conversion
      AdcScanModeFeature<>                        // scan mode with EOC after each group
    > _adc;

#endif

  public:

    void run() {

      _ready=false;

      /*
       * Subscribe to the interrupts raised by the ADC
       */

      _adc.AdcInterruptEventSender.insertSubscriber(
          AdcInterruptEventSourceSlot::bind(this,&AdcSingleInjected::onInterrupt)
        );

      /*
       * Enable auto mode for the injected channels. Auto mode means that the injected group
       * is converted after the regular group.
       */

      _adc.enableAutoInjectedMode();

      /*
       * Declare an instance of USART that we'll use to write out the conversion results.
       */

      Usart1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /**
       * Enable the ADC interrupt that tells us when the injected value has been converted. Because the
       * injected channel is converted after the regular channel we know that both values will be ready
       * to read.
       */

      _adc.enableInterrupts(Adc1InterruptFeature::INJECTED_END_OF_CONVERSION);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        // start a conversion

        _adc.startRegularConversion();

        while(!_ready);
        _ready=false;

        /*
         * write the values to the USART
         */

        outputStream << "Converted values are "
                     << StringUtil::Ascii(_values[0]) << ", "
                     << StringUtil::Ascii(_values[1]) << "\r\n";

        // wait for a second before converting the next set of values

        MillisecondTimer::delay(1000);
      }
    }


    /**
     * Interrupt callback will be fired when the injected value has been converted and
     * is ready for consumption. We'll read the regular and injected values out and signal to the
     * main context that the data is ready.
     * @param eventType Which interrupt was fired, see the AdcEventType enumeration for details.
     * @param adcNumber The ADC peripheral number that raised the interrupt (1..3). Will always be 1 in this test.
     */

    void onInterrupt(AdcEventType eventType,uint8_t adcNumber) {

      if(adcNumber==1 && eventType==AdcEventType::EVENT_INJECTED_END_OF_CONVERSION) {

        _values[0]=_adc.getRegularConversionValue();
        _values[1]=_adc.getInjectedConversionValue(0);

        _ready=true;
      }
    }
};


int main() {

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  MillisecondTimer::initialise();

  AdcSingleInjected adc;
  adc.run();

  // not reached
  return 0;
}
