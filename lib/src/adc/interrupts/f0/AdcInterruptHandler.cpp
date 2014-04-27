/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/adc.h"


#if defined(STM32PLUS_F0)

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

AdcInterruptFeature::FPTR AdcInterruptFeature::_forceLinkage=nullptr;
AdcEventSource *AdcInterruptFeature ::_adcInstance=nullptr;


extern "C" {

  #if defined(USE_ADC_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) ADC1_COMP_IRQHandler(void) {

      uint16_t raised1;

      // ADC1 interrupts - the more likely to be a source. we'll prioritise these checks so
      // the most likely are checked first and optimise slightly by not repeatedly calling
      // the inefficient ADC_GetITStatus call

      raised1=ADC1->ISR & (ADC_ISR_EOC | ADC_ISR_EOSEQ | ADC_ISR_AWD | ADC_ISR_OVR | ADC_ISR_EOSMP | ADC_ISR_ADRDY);

      if((raised1 & ADC_ISR_EOC)!=0 && (ADC1->IER & ADC_IER_EOCIE)!=0) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_REGULAR_END_OF_CONVERSION,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
      }
      else if((raised1 & ADC_ISR_OVR)!=0 && (ADC1->IER & ADC_IER_OVRIE)!=0) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_OVERFLOW,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
      }
      else if((raised1 & ADC_ISR_EOSEQ)!=0 && (ADC1->IER & ADC_IER_EOSEQIE)!=0) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_END_OF_SEQUENCE,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_EOSEQ);
      }
      else if((raised1 & ADC_ISR_AWD)!=0 && (ADC1->IER & ADC_IER_AWDIE)!=0) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_ANALOG_WATCHDOG,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_AWD);
      }
      else if((raised1 & ADC_ISR_EOSMP)!=0 && (ADC1->IER & ADC_IER_EOSMPIE)!=0) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_END_OF_SAMPLING,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_EOSMP);
      }
      else if((raised1 & ADC_ISR_ADRDY)!=0 && (ADC1->IER & ADC_IER_ADRDYIE)!=0) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_ADC_READY,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_ADRDY);
      }

      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

  #endif
}

#endif
