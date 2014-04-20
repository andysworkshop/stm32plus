/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"
#include "config/adc.h"

#if defined(STM32PLUS_F4) || defined (STM32PLUS_F1)

namespace stm32plus {

  uint8_t Adc::_regularChannelRank[3];
  uint8_t Adc::_injectedChannelRank[3];
}


#endif
