/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timing.h"
#include "config/adc.h"
#include "config/timer.h"
#include "config/usart.h"
#include "config/string.h"


using namespace stm32plus;


/**
 * This example shows how to use a timer to trigger the ADC conversion. We set up Timer 3 to generate an
 * update event once per second. We use this update event to trigger a single channel conversion on ADC1.
 * We use the ADC1 end-of-conversion (EOC) interrupt to tell us when the conversion is ready. We then log
 * that converted value to the USART where you can see it using normal PC terminal software.
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


class AdcSingleTimerInterrupts {

  protected:
    bool _ready;

  public:

    void run() {

      /*
       * Reset the interrupt ready flag
       */

      _ready=false;

      /*
       * Declare Timer3
       */

      Timer3<Timer3InternalClockFeature> timer;

      /*
       * Set an up-timer up to tick at 8kHz with an auto-reload value of 7999
       * The timer will count from 0 to 7999 inclusive then reset back to 0.
       * It will take exactly 1 second to do this. The timer generates its update
       * event when it wraps around to zero.
       */

      timer.setTimeBaseByFrequency(8000,7999);

#if defined(STM32PLUS_F0)

      /*
       * Declare the ADC peripheral with the 14MHz dedicated clock and a resolution of
       * 12 bits. We will use 144-cycle conversions on ADC channel 0. We trigger the conversion
       * using the rising edge of Timer 3's trigger signal and we want the trigger to be linked
       * to the timer's update event.
       */

      Adc1<
        AdcAsynchronousClockModeFeature,                          // the free-running 14MHz HSI
        AdcResolutionFeature<12>,                                 // 12 bit resolution
        Adc1Cycle28RegularChannelFeature<0>,                      // using channel 0 (PA0) on ADC1 with 28.5 cycle latency
        AdcTimer3TriggerRisingFeature<AdcTriggerSource::Update>,  // using timer 3 trigger-on-update
        Adc1InterruptFeature
      > adc;

#elif defined(STM32PLUS_F1)

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2 and a resolution of
       * 12 bits. We will use 144-cycle conversions on ADC channel 0. We trigger the conversion
       * using the rising edge of Timer 3's trigger signal and we want the trigger to be linked
       * to the timer's update event.
       */

      Adc1<
        AdcClockPrescalerFeature<6>,                              // PCLK2/6
        Adc1Cycle55RegularChannelFeature<0>,                      // using channel 0 on ADC1 with 55.5-cycle latency
        AdcTimer3TriggerRisingFeature<AdcTriggerSource::Update>,  // using timer 3 trigger-on-update
        Adc1InterruptFeature
      > adc;

#elif defined(STM32PLUS_F4)

      /*
       * Declare the ADC peripheral with an APB2 clock prescaler of 2 and a resolution of
       * 12 bits. We will use 144-cycle conversions on ADC channel 0. We trigger the conversion
       * using the rising edge of Timer 3's trigger signal and we want the trigger to be linked
       * to the timer's update event.
       */

      Adc1<
        AdcClockPrescalerFeature<2>,                              // prescaler of 2
        AdcResolutionFeature<12>,                                 // 12 bit resolution
        Adc1Cycle144RegularChannelFeature<0>,                     // using channel 0 on ADC1 with 144-cycle latency
        AdcTimer3TriggerRisingFeature<AdcTriggerSource::Update>,  // using timer 3 trigger-on-update
        Adc1InterruptFeature
      > adc;

#endif

      /*
       * Subscribe to the interrupts raised by the ADC
       */

      adc.AdcInterruptEventSender.insertSubscriber(
          AdcInterruptEventSourceSlot::bind(this,&AdcSingleTimerInterrupts::onInterrupt)
        );

      /*
       * Declare an instance of USART that we'll use to write out the conversion results.
       */

      Usart1<> usart(57600);
      UsartPollingOutputStream outputStream(usart);

      /**
       * Enable the ADC interrupts
       */

      adc.enableInterrupts(Adc1InterruptFeature::END_OF_CONVERSION);

      /*
       * Start the timer
       */

      timer.enablePeripheral();

      /*
       * Wait for the interrupt handler to notify us then log the converted
       * value and the time since the last conversion
       */

      uint32_t now;
      uint16_t value;

      for(;;) {

        while(!_ready);
        _ready=false;

        value=adc.getRegularConversionValue();

        now=MillisecondTimer::millis();

        outputStream << "Converted value is " << StringUtil::Ascii(value)
                     << " at " << StringUtil::Ascii(now) << "ms.\r\n";
      }
    }


    /**
     * Interrupt handler for the DMA complete event. Set the ready flag
     * when it's received.
     */

    void onInterrupt(AdcEventType eventType,uint8_t adcNumber) {

      if(adcNumber==1 && eventType==AdcEventType::EVENT_REGULAR_END_OF_CONVERSION)
        _ready=true;
    }
};


int main() {

  // we're using interrupts, initialise NVIC and the SysTick timer

  Nvic::initialise();
  MillisecondTimer::initialise();

  AdcSingleTimerInterrupts adc;
  adc.run();

  // not reached
  return 0;
}
