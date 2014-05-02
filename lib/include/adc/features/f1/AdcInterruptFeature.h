/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


/*
 * Forward declare the IRQ handler name. There is a single IRQ handler
 * for ADC 1,2,3 on the F4
 */

extern "C" void ADC_IRQHandler();


namespace stm32plus {


  /**
   * Feature class to allow allow handling of ADC interrupts. Declare this feature
   * with your AdcN<> declaration, register an event handler with the AdvEventSource
   * base class and then use enableInterrupts() to start.
   */

  class AdcInterruptFeature : public AdcEventSource,
                              public AdcFeatureBase {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    protected:
      uint16_t _interruptMask;

    public:
      enum {
        END_OF_CONVERSION           = 0x01,
        INJECTED_END_OF_CONVERSION  = 0x02,         // overflow is not present on the F1
        ANALOG_WATCHDOG             = 0x08
      };

      static AdcEventSource *_adcInstance;

    public:
      AdcInterruptFeature(Adc& adc);
      ~AdcInterruptFeature();

      void initialise() {}

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);
  };


  /**
   * Typedefs for each ADC. Even though they are all the same on the F4 this is
   * for compatibility with the F1 where there are multiple.
   */

  typedef AdcInterruptFeature Adc1InterruptFeature;
  typedef AdcInterruptFeature Adc2InterruptFeature;
  typedef AdcInterruptFeature Adc3InterruptFeature;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  inline AdcInterruptFeature::AdcInterruptFeature(Adc& adc)
    : AdcFeatureBase(adc) {

    _interruptMask=0;
    _adcInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  inline AdcInterruptFeature::~AdcInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. END_OF_CONVERSION | OVERFLOW
   */

  inline void AdcInterruptFeature::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    _forceLinkage=&ADC_IRQHandler;

#if defined(STM32PLUS_F1_MD_VL)
    Nvic::configureIrq(ADC1_IRQn);
#else
    Nvic::configureIrq(ADC1_2_IRQn);
#endif

    if((interruptMask & END_OF_CONVERSION)!=0)
      ADC_ITConfig(_adc,ADC_IT_EOC,ENABLE);
    if((interruptMask & INJECTED_END_OF_CONVERSION)!=0)
      ADC_ITConfig(_adc,ADC_IT_JEOC,ENABLE);
    if((interruptMask & ANALOG_WATCHDOG)!=0)
      ADC_ITConfig(_adc,ADC_IT_AWD,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. END_OF_CONVERSION | OVERFLOW
   */

  inline void AdcInterruptFeature::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;

    if((interruptMask & END_OF_CONVERSION)!=0)
      ADC_ITConfig(_adc,ADC_IT_EOC,DISABLE);
    if((interruptMask & INJECTED_END_OF_CONVERSION)!=0)
      ADC_ITConfig(_adc,ADC_IT_JEOC,DISABLE);
    if((interruptMask & ANALOG_WATCHDOG)!=0)
      ADC_ITConfig(_adc,ADC_IT_AWD,DISABLE);
  }
}
