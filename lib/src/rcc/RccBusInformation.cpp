/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/rcc.h"


namespace stm32plus {


  #if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)

  /*
   * Get the APB1 prescaler value
   */

  uint16_t RccBusInformation::getAPB1Prescaler() {

    switch(RCC->CFGR & RCC_CFGR_PPRE1) {

      case RCC_CFGR_PPRE1_DIV1:
        return 1;

      case RCC_CFGR_PPRE1_DIV2:
        return 2;

      case RCC_CFGR_PPRE1_DIV4:
        return 4;

      case RCC_CFGR_PPRE1_DIV8:
        return 8;

      case RCC_CFGR_PPRE1_DIV16:
      default:
        return 16;
    }
  }


  /*
   * Get the APB2 prescaler value
   */

  uint16_t RccBusInformation::getAPB2Prescaler() {

    switch(RCC->CFGR & RCC_CFGR_PPRE2) {

      case RCC_CFGR_PPRE2_DIV1:
        return 1;

      case RCC_CFGR_PPRE2_DIV2:
        return 2;

      case RCC_CFGR_PPRE2_DIV4:
        return 4;

      case RCC_CFGR_PPRE2_DIV8:
        return 8;

      case RCC_CFGR_PPRE2_DIV16:
      default:
        return 16;
    }
  }


#elif defined (STM32PLUS_F0)


  /*
   * Get the APB2 prescaler value
   */

  uint16_t RccBusInformation::getAPBPrescaler() {

    switch(RCC->CFGR & RCC_CFGR_PPRE) {

      case RCC_CFGR_PPRE_DIV1:
        return 1;

      case RCC_CFGR_PPRE_DIV2:
        return 2;

      case RCC_CFGR_PPRE_DIV4:
        return 4;

      case RCC_CFGR_PPRE_DIV8:
        return 8;

      case RCC_CFGR_PPRE_DIV16:
      default:
        return 16;
    }
  }


#endif


  /*
   * Get the AHB prescaler value
   */

  uint16_t RccBusInformation::getAHBPrescaler() {

    switch(RCC->CFGR & RCC_CFGR_HPRE) {

      case RCC_CFGR_HPRE_DIV1:
        return 1;

      case RCC_CFGR_HPRE_DIV2:
        return 2;

      case RCC_CFGR_HPRE_DIV4:
        return 4;

      case RCC_CFGR_HPRE_DIV8:
        return 8;

      case RCC_CFGR_HPRE_DIV16:
        return 16;

      case RCC_CFGR_HPRE_DIV64:
        return 64;

      case RCC_CFGR_HPRE_DIV128:
        return 128;

      case RCC_CFGR_HPRE_DIV256:
        return 256;

      case RCC_CFGR_HPRE_DIV512:
      default:
        return 512;
    }
  }
}
