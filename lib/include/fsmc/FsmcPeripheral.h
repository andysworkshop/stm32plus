/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * base class for FSMC implementations
   * @tparam TBankIdentifier FSMC_Bank1_NORSRAM1 etc...
   */

  template<uint32_t TBankIdentifier> class FsmcPeripheral;
}
