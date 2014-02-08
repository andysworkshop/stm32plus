/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/nvic.h"


namespace stm32plus {

  /**
   * The singleton that can be referenced from anywhere in the app
   */

  Nvic nvicController;
}
