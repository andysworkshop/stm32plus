/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/rng.h"


using namespace stm32plus;


/**
 * Template static data member initialisation
 */

RngEventSource *RngInterruptFeature::_rngInstance;


// static initialisers for the hack that forces the IRQ handlers to be linked

RngInterruptFeatureEnabler::FPTR RngInterruptFeatureEnabler::_forceLinkage=nullptr;


#if defined(USE_RNG_INTERRUPT)

extern "C" {

  /**
   * IRQ handler for HASH and RNG
   */

  void __attribute__ ((interrupt("IRQ"))) HASH_RNG_IRQHandler(void) {

    if(RNG_GetITStatus(RNG_IT_CEI)!=RESET) {
      RngInterruptFeature::_rngInstance->RngInterruptEventSender.raiseEvent(RngEventType::EVENT_CLOCK_ERROR,0);
      RNG_ClearITPendingBit(RNG_IT_CEI);
    }
    else if(RNG_GetITStatus(RNG_IT_SEI)!=RESET) {
      RngInterruptFeature::_rngInstance->RngInterruptEventSender.raiseEvent(RngEventType::EVENT_SEED_ERROR,0);
      RNG_ClearITPendingBit(RNG_IT_SEI);
    }
    else if(RNG_GetFlagStatus(RNG_FLAG_DRDY))
      RngInterruptFeature::_rngInstance->RngInterruptEventSender.raiseEvent(RngEventType::EVENT_DATA_READY,RNG_GetRandomNumber());

    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}


#endif
#endif
