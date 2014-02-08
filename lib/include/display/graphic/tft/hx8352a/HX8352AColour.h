/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace display {


    /**
     * Empty generic class definition for HX8352A colour-dependent operations
     * Everything is provided in the specialisations.
     */

    template<ColourDepth TColour,class TAccessMode,class TPanelTraits>
    class HX8352AColour;
  }
}


/*
 * Now include the specialisations
 */

#include "display/graphic/tft/hx8352a/HX8352AColour18Specialisation.h"
#include "display/graphic/tft/hx8352a/HX8352AColour16Specialisation.h"
