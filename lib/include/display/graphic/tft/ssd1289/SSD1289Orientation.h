/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Empty generic class definition for SSD1289 orientation-dependent operations
     * Everything is provided in the specialisations.
     */

    template<Orientation TOrientation,class TAccessMode>
    class SSD1289Orientation {
    };
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/ssd1289/SSD1289PortraitSpecialisation.h"
#include "display/graphic/tft/ssd1289/SSD1289LandscapeSpecialisation.h"
