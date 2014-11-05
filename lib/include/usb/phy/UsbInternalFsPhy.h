/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /*
   * Internal full-speed PHY implementation, inherits from the PHY base functionality
   */

  class UsbInternalFsPhy {

    public:
      struct Parameters {
      };

    public:
      UsbInternalFsPhy(Parameters& /* params */) {
      }
  };
}
