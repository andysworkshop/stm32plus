/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {

    /**
     * Empty generic class definition for ST7783 colour-dependent operations
     * Everything is provided in the specialisations.
     */

    template<ColourDepth TColour,class TAccessMode>
    class ST7783Colour;
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/st7783/ST7783Colour16Specialisation.h"
#include "display/graphic/tft/st7783/ST7783Colour18Specialisation.h"
