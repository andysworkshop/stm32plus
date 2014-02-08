/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Nothing class to allow construction of CS43L22 without a digital data source
   */

  struct NullCS43L22DataPeripheral {

    struct Parameters {
    };

    NullCS43L22DataPeripheral(const Parameters&) {
    }
  };
}

