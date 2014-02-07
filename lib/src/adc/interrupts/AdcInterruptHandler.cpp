/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/adc.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

AdcInterruptFeature::FPTR AdcInterruptFeature::_forceLinkage=nullptr;
AdcEventSource *AdcInterruptFeature ::_adcInstance=nullptr;


extern "C" {

  #if defined(USE_ADC_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) ADC_IRQHandler(void) {

      // ADC1 interrupts - the more likely to be a source. we'll prioritise these checks so
      // the most likely are checked first.

      if(ADC_GetITStatus(ADC1,ADC_IT_EOC)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_REGULAR_END_OF_CONVERSION,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
      }
      else if(ADC_GetITStatus(ADC1,ADC_IT_JEOC)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_INJECTED_END_OF_CONVERSION,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);
      }
      else if(ADC_GetITStatus(ADC1,ADC_IT_AWD)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_ANALOG_WATCHDOG,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_AWD);
      }
      else if(ADC_GetITStatus(ADC1,ADC_IT_OVR)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_OVERFLOW,1);
        ADC_ClearITPendingBit(ADC1,ADC_IT_OVR);
      }

      // ADC2 interrupts

      else if(ADC_GetITStatus(ADC2,ADC_IT_EOC)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_REGULAR_END_OF_CONVERSION,2);
        ADC_ClearITPendingBit(ADC2,ADC_IT_EOC);
      }
      else if(ADC_GetITStatus(ADC2,ADC_IT_JEOC)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_INJECTED_END_OF_CONVERSION,2);
        ADC_ClearITPendingBit(ADC2,ADC_IT_JEOC);
      }
      else if(ADC_GetITStatus(ADC2,ADC_IT_AWD)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_ANALOG_WATCHDOG,2);
        ADC_ClearITPendingBit(ADC2,ADC_IT_AWD);
      }
      else if(ADC_GetITStatus(ADC2,ADC_IT_OVR)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_OVERFLOW,2);
        ADC_ClearITPendingBit(ADC2,ADC_IT_OVR);
      }

      // ADC3 interrupts

      else if(ADC_GetITStatus(ADC3,ADC_IT_EOC)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_REGULAR_END_OF_CONVERSION,3);
        ADC_ClearITPendingBit(ADC3,ADC_IT_EOC);
      }
      else if(ADC_GetITStatus(ADC3,ADC_IT_JEOC)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_INJECTED_END_OF_CONVERSION,3);
        ADC_ClearITPendingBit(ADC3,ADC_IT_JEOC);
      }
      else if(ADC_GetITStatus(ADC3,ADC_IT_AWD)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_ANALOG_WATCHDOG,3);
        ADC_ClearITPendingBit(ADC3,ADC_IT_AWD);
      }
      else if(ADC_GetITStatus(ADC3,ADC_IT_OVR)!=RESET) {
        AdcInterruptFeature::_adcInstance->AdcInterruptEventSender.raiseEvent(AdcEventType::EVENT_OVERFLOW,3);
        ADC_ClearITPendingBit(ADC3,ADC_IT_OVR);
      }

      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

  #endif
}
