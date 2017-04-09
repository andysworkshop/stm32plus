/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Internal flash device features class. This is the class that user
   * code should instantiate
   */

  template<class TSectorMap,class... Features>
  struct InternalFlashDevice : InternalFlashPeripheral,Features... {

    /**
     * Constructor
     */

    InternalFlashDevice()
      : InternalFlashPeripheral(TSectorMap::SizeMap),Features(static_cast<InternalFlashPeripheral&>(*this))... {
    }
  };
}
