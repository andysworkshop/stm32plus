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
 * This example illustrates the use of interrupts to signal the end of a conversion. Using an interrupt
 * can be a more efficient way to manage the flow of converted data if your MCU has other things to do
 * such as being responsive to the actions going on in a user interface.
 *
 * ADC1 is configured with 12-bit resolution and, on the F4, an APB2 clock prescaler of 2 and a 56-cycle
 * conversion time. On the F0 we use a 7.5 cycle conversion time against PCLK/2.
 * We will use channel #0. USART1 is configured with 57600/8/N/1 parameters.
 *
 * To run this example you can connect PA0 (ADC123_IN0) to see a conversion value of 0 or you can
 * connect PA0 to the VREF level (probably 3.3V or 3V) to see a conversion value of 4095. The actual
 * values that you get will vary according to the noise present on the line.
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
 *   STM32F107VCT6
 */

class AdcSingleInterrupts {

  protected:
    volatile bool _ready;           // set by the interrupt callback when data is ready (EOC)
    volatile bool _error;           // set by the interrupt callback if there's an overflow error (OVR)

  public:

    void run() {

      _ready=_error=false;

#if defined(STM32PLUS_F0)

      /*
       * Declare the ADC peripheral using PCLK with a prescaler of 2, a resolution of
       * 12 bits. We will use 55.5-cycle conversions on ADC channel 0.
       */

      Adc1<
        AdcPclk2ClockModeFeature,                 // prescaler of 2
        AdcResolutionFeature<12>,                 // 12 bit resolution
        Adc1Cycle7RegularChannelFeature<0>,       // using channel 0 on ADC1 with 7.5-cycle latency
        Adc1InterruptFeature                      // enable interrupt handling on this ADC
        > adc;

#elif defined(STM32PLUS_F4)

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
       * 12 bits. We will use 3-cycle conversions on ADC channel 0.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,              // prescaler of 2
        AdcResolutionFeature<12>,                 // 12 bit resolution
        Adc1Cycle56RegularChannelFeature<0>,      // using channel 0 on ADC1 with 56-cycle latency
        Adc1InterruptFeature                      // enable interrupt handling on this ADC
        > adc;

#endif

      /*
       * Subscribe to the interrupts raised by the ADC
       */

      adc.AdcInterruptEventSender.insertSubscriber(
          AdcInterruptEventSourceSlot::bind(this,&AdcSingleInterrupts::onInterrupt)
        );

      /*
       * Declare an instance of USART that we'll use to write out the conversion results.
       */

      Usart1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /**
       * Enable the ADC interrupts
       */

      adc.enableInterrupts(Adc1InterruptFeature::END_OF_CONVERSION | Adc1InterruptFeature::OVERFLOW);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        uint16_t value;

        // start a conversion

        adc.startRegularConversion();

        // wait for the interrupt handler to tell us that the conversion is done
        // then reset the ready flag

        while(!_ready) {
          if(_error)
            for(;;);            // lock up on error
        }
        _ready=false;

        // get the result

        value=adc.getRegularConversionValue();

        // write the value to the USART

        outputStream << "Converted value is " << StringUtil::Ascii(value) << "\r\n";

        // wait for a second before converting the next value

        MillisecondTimer::delay(1000);
      }
    }


    /**
     * Interrupt callback will be fired when a value has been converted and
     * is ready for consumption. We'll just signal to the main code that it
     * can wake up and pick up the value.
     * @param eventType Which interrupt was fired, see the AdcEventType enumeration for details.
     * @param adcNumber The ADC peripheral number that raised the interrupt (1..3). Will always be 1 in this test.
     */

    void onInterrupt(AdcEventType eventType,uint8_t adcNumber) {

      if(adcNumber==1) {

        if(eventType==AdcEventType::EVENT_REGULAR_END_OF_CONVERSION)
          _ready=true;
        else if(eventType==AdcEventType::EVENT_OVERFLOW)
          _error=true;
      }
    }
};


/*
 * Main entry point
 */

int main() {

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  MillisecondTimer::initialise();

  AdcSingleInterrupts adc;
  adc.run();

  // not reached
  return 0;
}
