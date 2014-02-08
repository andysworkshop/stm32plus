/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Traits class for the ILI9327 that declares a "short" panel
     * of 400x240, less than the driver maximum of 432x240
     */

    class ILI9327400x240PanelTraits {

      public:
        enum {
          LONG_SIDE = 400,
          SHORT_SIDE = 240
        };
    };
  }
}
