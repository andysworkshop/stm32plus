/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/adc.h"
#include "config/dac.h"
#include "config/usart.h"
#include "config/string.h"


using namespace stm32plus;


/**
 * This example shows how to use the analog watchdog. The analog watchdog allows you to set an acceptable voltage
 * range for one or all of your ADC channels, for example the VBat channel. If the level falls outside the acceptable
 * range then an interrupt is fired. Continuous conversion mode could be used along with interrupts in order to
 * do everything in the background although care must be taken in your code to avoid an infinite interrupt loop.
 *
 * We will set an acceptable range of 750-2500. We will configure DAC1 to output values of 0,1000,2000,3000,4000.
 * The output of DAC1 (PA4) will be connected to the input of ADC1 channel 0 (PA0).
 *
 * Connect the output of DAC1 (PA4) to the input of ADC channel 0 (PA0). Also connect PA9/PA10 to see the test
 * output on USART1.
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

class AdcAnalogWatchdog {

  protected:
    /*
     * Declare an instance of USART1 (TX/RX = PA9/PA10) that we'll use to write out
     * the results of the test
     */

    Usart1<> _usart;
    UsartPollingOutputStream _outputStream;


  public:

    /*
     * Constructor: initialise the USART
     */

    AdcAnalogWatchdog()
      : _usart(57600),_outputStream(_usart) {

    }


    /*
     * Run the test
     */

    void run() {

      /*
       * Configure DAC1 on PA4 and set the output value to 1500 so the AWD doesn't fire as
       * soon as the ADC starts up
       */

      Dac1<DacChannel112BitRightAlignmentFeature> dac;

      dac.write(1500);
      dac.enablePeripheral();

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
        Adc1Cycle28RegularChannelFeature<0>,      // using channel 0 (PA0) on ADC1 with 28.5 cycle latency
        AdcSingleChannelAnalogWatchdogFeature<0,750,2500>,    // using the AWD on channel 0 with a guarded range of 1000 to 2000
        AdcInterruptFeature                       // interrupts are raised by the AWD
        > adc;

#elif defined(STM32PLUS_F1)


#elif defined(STM32PLUS_F4)

#endif

      /*
       * Subscribe to the interrupts raised by the ADC
       */

      adc.AdcInterruptEventSender.insertSubscriber(
          AdcInterruptEventSourceSlot::bind(this,&AdcAnalogWatchdog::onInterrupt)
        );

      /*
       * Enable the ADC interrupts and the watchdog
       */

      adc.enableInterrupts(Adc1InterruptFeature::ANALOG_WATCHDOG);
      adc.enableAnalogWatchdog();

      /*
       * Go into an infinite loop
       */

      for(;;) {

        uint16_t i;

        for(i=0;i<=4000;i+=1000) {

          // write out the value to the DAC pin

          _outputStream << "Writing " << StringUtil::Ascii(i) << " to the DAC\r\n";
          dac.write(i);

          // do a conversion

          adc.startRegularConversion();

          // pause for a second. if the value written to the DAC is outside the configured AWD
          // range then the interrupt will fire and an alert will be written to the USART.

          MillisecondTimer::delay(1000);
        }
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

      if(adcNumber==1 && eventType==AdcEventType::EVENT_ANALOG_WATCHDOG)
        _outputStream << "Analog watchdog interrupt fired\r\n";
    }
};


/*
 * Main entry point
 */

int main() {

  // we're using interrupts, initialise NVIC

  Nvic::initialise();

  MillisecondTimer::initialise();

  AdcAnalogWatchdog adc;
  adc.run();

  // not reached
  return 0;
}
