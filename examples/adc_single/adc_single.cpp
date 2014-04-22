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
 * This example illustrates the most basic of all uses of the ADC. A 12-bit conversion is performed
 * of a single channel. The conversion is done synchronously, on-demand and the result is written to
 * USART1.
 *
 * On the F4, ADC1 is configured with 12-bit resolution and an APB2 clock prescaler of 2 and a 3-cycle
 * conversion time. On the F0, ADC1 is configured to use the 14MHz dedicated internal oscillator with
 * a latency of 28.5 cycles.
 *
 * We will use channel 0 which is connected to PA0. USART1 is configured with 57600/8/N/1 parameters.
 *
 * To run this example you can connect PA0 (ADC123_IN0) to see a conversion value of 0 or you can
 * connect PA0 to the VREF level (probably 3.3V or 3V) to see a conversion value of 4095. The actual
 * values that you get will vary according to the noise present on the line.
 *
 * Compatible MCU:
 *   STM32F1
 *   STM32F4
 *
 * Tested on devices:
 *   STM32F100RBT6
 *   STM32F103ZET6
 *   STM32F407VGT6
 *   STM32F107VCT6
 */

class AdcSingle {

  public:

    void run() {

      /*
       * Unfortunately the ADC is quite different across the MCU series so we have to
       * be MCU-specific when declaring this instance
       */

#if defined(STM32PLUS_F0)

      /*
       * Declare the ADC peripheral with the 14MHz dedicated clock, a resolution of
       * 12 bits. We will use 28.5-cycle conversions on ADC channel 0.
       */

      Adc1<
        AdcAsynchronousClockModeFeature,          // the free-running 14MHz HSI
        AdcResolutionFeature<12>,                 // 12 bit resolution
        Adc1Cycle28RegularChannelFeature<0>       // using channel 0 (PA0) on ADC1 with 28.5 cycle latency
        > adc;

#elif defined(STM32PLUS_F4)

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2, a resolution of
       * 12 bits. We will use 3-cycle conversions on ADC channel 0.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,              // prescaler of 2
        AdcResolutionFeature<12>,                 // 12 bit resolution
        Adc1Cycle3RegularChannelFeature<0>        // using channel 0 (PA0) on ADC1 with 3-cycle latency
        > adc;
#endif

      /*
       * Declare an instance of USART that we'll use to write out the conversion results.
       */

      Usart1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /*
       * Go into an infinite loop converting
       */

      for(;;) {

        uint16_t value;

        // start a conversion manually

        adc.startRegularConversion();

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
};


/*
 * Main entry point
 */

int main() {

  MillisecondTimer::initialise();

  AdcSingle adc;
  adc.run();

  // not reached
  return 0;
}
