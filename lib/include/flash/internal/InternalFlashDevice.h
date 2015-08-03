/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Internal flash device features class. This is the class that user
   * code should instantiate
   */

  template<class... Features>
  struct InternalFlashDevice : Features... {
  };
}
